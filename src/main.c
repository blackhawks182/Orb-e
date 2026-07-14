#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define ORB_SIZE 20.0f
#define MOVE_SPEED 350.0f // Speed of direct movement in pixels per second

typedef struct Orb {
    Vector2 position;
    float radius;
    float rotation; // Maintained purely to point the visual indicator where you move
} Orb;


int main(void) {
    // 1. Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orb-e");
    SetTargetFPS(60);

	Orb player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = ORB_SIZE;
    player.rotation = 0.0f; // Starts facing up

    
    // 2. Main Game Loop
    while (!WindowShouldClose()) {
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

        
        // ---------------------------------------------------------------------
        // RENDER
        // ---------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        
        EndDrawing();
    }

    // 3. De-Initialization
    CloseWindow();
    return 0;
}