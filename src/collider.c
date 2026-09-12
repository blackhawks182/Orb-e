#include "raylib.h"

#include "raymath.h"

#include "resource_dir.h"    // utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

#define colliderOrb_size 25.0f

#define start

#define maxSpeed 1000.0f

#define minSpeed 100.0f

#define speedSpeed 500.0f

#define jumpSpeed 500.0f

#define gravity 1000.0f

#define startSpeed 100.0f

#define startMass 1.0f

#define start_X 200.0f

#define start_Y 500.0f

void startCollider(void)
{
    typedef struct colliderOrb{
        Vector2 position;
        Vector2 velocity;
        float mass;
        int inAir;
    } colliderOrb;

    Rectangle ground = {0, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
    colliderOrb Orb;
    Orb.position = (Vector2){start_X,start_Y};
    Orb.velocity = (Vector2){startSpeed, 0};
    Orb.mass = startMass;
    Orb.inAir = 0;
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO))
    {
        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_D) && Orb.velocity.x < maxSpeed) Orb.velocity.x += speedSpeed * deltaTime;
        if (IsKeyDown(KEY_A) && Orb.velocity.x > minSpeed) Orb.velocity.x -= speedSpeed * deltaTime;
        if (IsKeyPressed(KEY_SPACE) && !Orb.inAir){
            Orb.inAir = 1;
            Orb.velocity.y = -jumpSpeed;
        }
        Orb.position.x += Orb.velocity.x * deltaTime;

        if (Orb.inAir){
            Orb.velocity.y += gravity * deltaTime;
            Orb.position.y += Orb.velocity.y * deltaTime;

            if (Orb.position.y + colliderOrb_size >= SCREEN_HEIGHT*2/3){
                Orb.position.y = SCREEN_HEIGHT*2/3 - colliderOrb_size;
                Orb.velocity.y = 0;
                Orb.inAir = 0;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);
        DrawCircleV(Orb.position, colliderOrb_size, RED);
        DrawText(TextFormat("Speed: %.0f", Orb.velocity.x), Orb.position.x - 50, Orb.position.y - 50, 20, WHITE);
        EndDrawing();
    }    
}
