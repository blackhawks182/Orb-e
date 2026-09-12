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

#define start_Y SCREEN_HEIGHT*2/3 - colliderOrb_size

void startCollider(void)
{
    typedef struct colliderOrb{
        Vector2 position;
        Vector2 velocity;
        float mass;
        int inAir;
    } colliderOrb;

    typedef struct enemyOrb{
        Vector2 position;
        Vector2 velocity;
        float mass;
        int active;
    } enemyOrb;

    Rectangle ground = {0, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
    colliderOrb Orb;
    enemyOrb Enemy;
    Enemy.position = (Vector2){1000, SCREEN_HEIGHT*2/3 - colliderOrb_size};
    Enemy.velocity = (Vector2){-150, 0};
    Enemy.mass = 1.0f;
    Enemy.active = 1;

    Orb.position = (Vector2){start_X,start_Y};
    Orb.velocity = (Vector2){startSpeed, 0};
    Orb.mass = startMass;
    Orb.inAir = 0;
    float groundPosition = 0.0f;
    Color rec1 = GRAY;
    Color rec2 = GREEN;
    Color tempColor;
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO))
    {
        float deltaTime = GetFrameTime();

        if (IsKeyDown(KEY_D) && Orb.velocity.x < maxSpeed) Orb.velocity.x += speedSpeed * deltaTime;
        if (Orb.velocity.x > maxSpeed) Orb.velocity.x = maxSpeed;
        if (IsKeyDown(KEY_A) && Orb.velocity.x > minSpeed) Orb.velocity.x -= speedSpeed * deltaTime;
        if (Orb.velocity.x < minSpeed) Orb.velocity.x = minSpeed;
        if (IsKeyPressed(KEY_SPACE) && !Orb.inAir){
            Orb.inAir = 1;
            Orb.velocity.y = -jumpSpeed;
        }
        float orbMomentum = Orb.mass * Orb.velocity.x;
        float enemyMomentum = Enemy.mass * -Enemy.velocity.x;
        if (Enemy.active){
            Enemy.position.x += Enemy.velocity.x * deltaTime;
        }
        groundPosition -= Orb.velocity.x * deltaTime;
        if (groundPosition <= -SCREEN_WIDTH) {
            groundPosition = 0;
            tempColor = rec1;
            rec1 = rec2;
            rec2 = tempColor;
        }


        if (Orb.inAir){
            Orb.velocity.y += gravity * deltaTime;
            Orb.position.y += Orb.velocity.y * deltaTime;

            if (Orb.position.y + colliderOrb_size >= SCREEN_HEIGHT*2/3){
                Orb.position.y = SCREEN_HEIGHT*2/3 - colliderOrb_size;
                Orb.velocity.y = 0;
                Orb.inAir = 0;
            }
        }
        if (Enemy.active && CheckCollisionCircles(Orb.position, colliderOrb_size, Enemy.position, colliderOrb_size)){
            if (Orb.mass * Orb.velocity.x > Enemy.mass * -Enemy.velocity.x){
                Enemy.active = 0;
                Orb.mass += Enemy.mass;
            }
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(groundPosition, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3, rec1);
        DrawRectangle(groundPosition + SCREEN_WIDTH, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3, rec2);
        DrawCircleV(Orb.position, colliderOrb_size, RED);
        if (Enemy.active)
            DrawCircleV(Enemy.position, colliderOrb_size, PURPLE);
        DrawText(TextFormat("Momentum: %.0f", orbMomentum), Orb.position.x - 60, Orb.position.y - 50, 20, WHITE);
        if (Enemy.active){
            DrawText(TextFormat("Momentum: %.0f", enemyMomentum), Enemy.position.x - 60, Enemy.position.y - 50, 20, WHITE);
        }
        EndDrawing();
    }    
}
