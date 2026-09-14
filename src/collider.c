#include "collider.h"

void startCollider(void)
{
    Rectangle ground = {0, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
    colliderOrb Orb;
    enemyOrb Enemy;
    Enemy.position = (Vector2){1100, SCREEN_HEIGHT*2/3 - enemy_size};
    Enemy.velocity = (Vector2){-GetRandomValue(100, 500), 0};
    Enemy.mass = GetRandomValue(5, 50) / 10.0f;
    Enemy.active = 1;

    Orb.position = (Vector2){start_X,start_Y};
    Orb.velocity = (Vector2){startSpeed, 0};
    Orb.mass = startMass;
    Orb.inAir = 0;
    float groundPosition = 0.0f;
    Color rec1 = GRAY;
    Color rec2 = GREEN;
    Color tempColor;
    int gameOver = 0;
    Texture2D groundTexture = LoadTexture("assets/cannon_throw_ground.png");
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO)&& !gameOver)
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
        if (IsKeyDown(KEY_SPACE) && Orb.inAir) Orb.velocity.y -= jumpSpeed * deltaTime;

        //float orbMomentum = Orb.mass * Orb.velocity.x;
        //float enemyMomentum = Enemy.mass * -Enemy.velocity.x;
        if (Enemy.active){
            Enemy.position.x += (Enemy.velocity.x - Orb.velocity.x) * deltaTime;
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
        if (Enemy.active && CheckCollisionCircles(Orb.position, colliderOrb_size, Enemy.position, enemy_size)){
            if (Orb.mass * Orb.velocity.x > Enemy.mass * -Enemy.velocity.x){
                Enemy.active = 0;
                Orb.mass += Enemy.mass*0.1f;
                Enemy.position = (Vector2){1100, SCREEN_HEIGHT*2/3 - enemy_size};
                Enemy.velocity = (Vector2){-GetRandomValue(100, 500), 0};
                Enemy.mass = GetRandomValue(5, 50) / 10.0f;
                Enemy.active = 1;
            }
            else gameOver = 1;
        }
        if (Enemy.active && Enemy.position.x + enemy_size < 0){
            Enemy.position = (Vector2){1100, GetRandomValue(150, SCREEN_HEIGHT*2/3 - enemy_size)};
            Enemy.velocity = (Vector2){-GetRandomValue(100, 500), 0};
            Enemy.mass = GetRandomValue(5, 50) / 10.0f;
            Enemy.active = 1;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        Rectangle rect1 = {groundPosition, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
        Rectangle rect2 = {groundPosition + SCREEN_WIDTH, SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
        DrawTexturePro(
            groundTexture,
            (Rectangle){0, 0, groundTexture.width, groundTexture.height},
            rect1,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        DrawTexturePro(
            groundTexture,
            (Rectangle){0, 0, groundTexture.width, groundTexture.height},
            rect2,
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        DrawCircleV(Orb.position, colliderOrb_size, RED);
        if (Enemy.active)
            DrawCircleV(Enemy.position, enemy_size, PURPLE);
        DrawText(TextFormat("Speed: %.0f", Orb.velocity.x), Orb.position.x - 50, Orb.position.y - 50, 20, WHITE);
        DrawText(TextFormat("Mass: %.1f", Orb.mass), Orb.position.x - 50, Orb.position.y - 25, 20, WHITE);

        if (Enemy.active){
            DrawText(TextFormat("Speed: %.0f", -Enemy.velocity.x), Enemy.position.x - 50, Enemy.position.y - 50, 20, WHITE);
            DrawText(TextFormat("Mass: %.1f", Enemy.mass), Enemy.position.x - 50, Enemy.position.y - 25, 20, WHITE);
        }
        EndDrawing();
    }    
}
