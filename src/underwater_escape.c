#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define ORB_SIZE 20.0f
#define MOVE_SPEED 350.0f // Speed of direct movement in pixels per second

#define MAX_SHURIKENS 20
#define SHURIKEN_SPEED 700.0f
#define SHURIKEN_LIFETIME 1.5f // Seconds before a shuriken disappears

typedef struct Orb {
    Vector2 position;
    float radius;
    float rotation; // Maintained purely to point the visual indicator where you move
} Orb;

typedef struct Shuriken {
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float rotation; // Rotation of the shuriken graphic itself
    bool active;
} Shuriken;

// Helper to wrap positions around screen edges
void WrapPosition(Vector2 *pos, float margin) {
    if (pos->x < -margin) pos->x = SCREEN_WIDTH + margin;
    if (pos->x > SCREEN_WIDTH + margin) pos->x = -margin;
    if (pos->y < -margin) pos->y = SCREEN_HEIGHT + margin;
    if (pos->y > SCREEN_HEIGHT + margin) pos->y = -margin;
}



void startUnderwaterEscape(void) {
    // 1. Initialization
    Orb player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = ORB_SIZE;
    player.rotation = 0.0f; // Starts facing up

    Shuriken shurikens[MAX_SHURIKENS] = {0};

    // 2. Main Game Loop
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO)) {
		float dt = GetFrameTime();

        
        // UPDATE MOVEMENT 
        
        Vector2 movement = { 0.0f, 0.0f };

        // Check horizontal movement
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  movement.x -= 1.0f;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) movement.x += 1.0f;

        // Check vertical movement
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    movement.y -= 1.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  movement.y += 1.0f;

        // If moving, apply normalization and update position/rotation
        if (movement.x != 0.0f || movement.y != 0.0f) {
            // Normalize the vector so diagonal movement isn't faster (traveling at sqrt(2))
            float length = sqrtf(movement.x * movement.x + movement.y * movement.y);
            movement.x /= length;
            movement.y /= length;

            // Direct position modification (no acceleration or drift)
            player.position.x += movement.x * MOVE_SPEED * dt;
            player.position.y += movement.y * MOVE_SPEED * dt;

            // Update rotation based on your move direction so the weapon face aligns
            // atan2f returns radians; we convert to degrees and adjust by +90 so 0 is up
            player.rotation = (atan2f(movement.y, movement.x) * RAD2DEG) + 90.0f;
        }

        // Screen edge wrapping
        WrapPosition(&player.position, player.radius);
		// Compute forward facing vector to draw the direction indicator line
        Vector2 forward = { 
            cosf((player.rotation - 90.0f) * DEG2RAD), 
            sinf((player.rotation - 90.0f) * DEG2RAD) 
        };

		
        // FIRING SHURIKENS

        if (IsKeyPressed(KEY_SPACE)) {
            for (int i = 0; i < MAX_SHURIKENS; i++) {
                if (!shurikens[i].active) {
                    // Position shuriken slightly outward at the tip of the ship
                    shurikens[i].position = (Vector2){
                        player.position.x + forward.x * player.radius,
                        player.position.y + forward.y * player.radius
                    };
                    
                    // Match the trajectory velocity with weapon speeds
                    shurikens[i].velocity = (Vector2){
                        forward.x * SHURIKEN_SPEED,
                        forward.y * SHURIKEN_SPEED
                    };
                    
                    shurikens[i].lifeTime = SHURIKEN_LIFETIME;
                    shurikens[i].rotation = 0.0f;
                    shurikens[i].active = true;
                    break; // Found an empty slot, stop looking!
                }
            }
        }

        // UPDATE PROJECTILES
        
        for (int i = 0; i < MAX_SHURIKENS; i++) {
            if (shurikens[i].active) {
                // Update translation positions
                shurikens[i].position.x += shurikens[i].velocity.x * dt;
                shurikens[i].position.y += shurikens[i].velocity.y * dt;
                
                // Spin the shuriken graphic rapidly over time (720 degrees per second)
                shurikens[i].rotation += 720.0f * dt;
                
                // Deactivate shuriken if it goes off-screen
                float margin = 25.0f; // Matches shuriken size so it clears the screen fully before vanishing
                if (shurikens[i].position.x < -margin || 
                     shurikens[i].position.x > SCREEN_WIDTH + margin || 
                     shurikens[i].position.y < -margin || 
                     shurikens[i].position.y > SCREEN_HEIGHT + margin) 
                     {
                       shurikens[i].active = false;
                     }

                // Age the shuriken and disable if lifespan runs out
                shurikens[i].lifeTime -= dt;
                if (shurikens[i].lifeTime <= 0) {
                    shurikens[i].active = false;
                }
            }
        }


        // RENDER
        
        BeginDrawing();
        ClearBackground(BLUE);

		// Draw Active Shurikens
        for (int i = 0; i < MAX_SHURIKENS; i++) {
            if (shurikens[i].active) {
                float shurikenSize = 25.0f;

                // Outer 4-pointed red blade structure
                DrawPoly(shurikens[i].position, 4, shurikenSize, shurikens[i].rotation, RED);

                // Center contrasting rings
                DrawCircleV(shurikens[i].position, 4.0f, MAROON);
                DrawCircleV(shurikens[i].position, 2.0f, ORANGE);
            }
        }


        // Draw Orb Body
        DrawCircleV(player.position, player.radius, RAYWHITE);

        // Draw Reactor Core Visual
       DrawCircleV(player.position, player.radius * 0.35f, GetColor(0xF2BE44FF));

        // Draw Nose Direction Pointer Line (points where you are currently traveling)
        Vector2 noseLineEnd = {
            player.position.x + forward.x * player.radius,
            player.position.y + forward.y * player.radius
        };
        DrawLineEx(player.position, noseLineEnd, 3.0f, RED);

        

        
        EndDrawing();
    }
}