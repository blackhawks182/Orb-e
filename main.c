#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define MAX_LASERS 20
#define MAX_ASTEROIDS 100
#define SHIP_SIZE 20.0f
#define LASER_SPEED 700.0f
#define LASER_LIFETIME 1.5f // Seconds before a laser disappears

typedef struct Ship {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float radius;
} Ship;

typedef struct Laser {
    Vector2 position;
    Vector2 velocity;
    float lifeTime;
    float rotation;
    bool active;
} Laser;

typedef struct Asteroid {
    Vector2 position;
    Vector2 velocity;
    float radius;
    int sizeClass; // 3 = Large, 2 = Medium, 1 = Small
    bool active;
} Asteroid;

// Helper to wrap positions around screen edges
void WrapPosition(Vector2 *pos, float margin) {
    if (pos->x < -margin) pos->x = SCREEN_WIDTH + margin;
    if (pos->x > SCREEN_WIDTH + margin) pos->x = -margin;
    if (pos->y < -margin) pos->y = SCREEN_HEIGHT + margin;
    if (pos->y > SCREEN_HEIGHT + margin) pos->y = -margin;
}

// Helper to spawn a single asteroid
void SpawnAsteroid(Asteroid *asteroids, Vector2 pos, int sizeClass) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i].position = pos;
            
            // Random direction and speed
            float angle = GetRandomValue(0, 360) * DEG2RAD;
            float speed = GetRandomValue(80, 150) / (float)sizeClass; // Smaller = faster
            asteroids[i].velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
            
            asteroids[i].sizeClass = sizeClass;
            asteroids[i].radius = sizeClass * 15.0f;
            asteroids[i].active = true;
            break;
        }
    }
}

int LoadHighScore()
{
    FILE *file = fopen("highscore.txt", "r");

    if (file == NULL)
        return 0;

    int highScore = 0;
    fscanf(file, "%d", &highScore);

    fclose(file);

    return highScore;
}


void SaveHighScore(int highScore)
{
    FILE *file = fopen("highscore.txt", "w");

    if (file == NULL)
        return;

    fprintf(file, "%d", highScore);

    fclose(file);
}

int main(void) {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids Clone - raylib");
    SetTargetFPS(60);

    // Game State variables
    Ship player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = SHIP_SIZE;
    
    Laser lasers[MAX_LASERS] = {0};
    Asteroid asteroids[MAX_ASTEROIDS] = {0};
    
    int score = 0;
    int highScore = LoadHighScore();
    int lives = 6;
    bool gameOver = false;

    // Initial Asteroid Spawning (Start with 5 large ones away from the center)
    for (int i = 0; i < 5; i++) {
        Vector2 spawnPos;
        do {
            spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
        } while (CheckCollisionCircles(spawnPos, 100, player.position, player.radius)); // Don't spawn on player
        
        SpawnAsteroid(asteroids, spawnPos, 3);
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // ---------------------------------------------------------------------
        // UPDATE GAME STATE
        // ---------------------------------------------------------------------
        if (!gameOver) {
            // 1. Player Rotation
            if (IsKeyDown(KEY_LEFT)) player.rotation -= 250.0f * dt;
            if (IsKeyDown(KEY_RIGHT)) player.rotation += 250.0f * dt;

            // 2. Player Thrust & Movement
            Vector2 forward = { 
    cosf((player.rotation - 90) * DEG2RAD), 
    sinf((player.rotation - 90) * DEG2RAD) 
};

// Forward thrust
if (IsKeyDown(KEY_UP))
{
    player.velocity.x += forward.x * 400.0f * dt;
    player.velocity.y += forward.y * 400.0f * dt;
}

// Reverse thrust
if (IsKeyDown(KEY_DOWN))
{
    player.velocity.x -= forward.x * 200.0f * dt;
    player.velocity.y -= forward.y * 200.0f * dt;
}
            
            // Apply friction/drag so the ship doesn't drift infinitely
            player.velocity.x *= 0.99f;
            player.velocity.y *= 0.99f;

            player.position.x += player.velocity.x * dt;
            player.position.y += player.velocity.y * dt;
            WrapPosition(&player.position, player.radius);

            // 3. Firing Lasers
            if (IsKeyPressed(KEY_SPACE)) {
                for (int i = 0; i < MAX_LASERS; i++) {
                    if (!lasers[i].active) {
                        // Position laser at the nose of the ship
                        lasers[i].position = Vector2Add(player.position, Vector2Scale(forward, player.radius));
                        lasers[i].velocity = Vector2Scale(forward, LASER_SPEED);
                        lasers[i].lifeTime = LASER_LIFETIME;
                        lasers[i].rotation = 0;
                        lasers[i].active = true;
                        break;
                    }
                }
            }

            // 4. Update Lasers
            for (int i = 0; i < MAX_LASERS; i++) {
                if (lasers[i].active) {
                    lasers[i].position.x += lasers[i].velocity.x * dt;
                    lasers[i].position.y += lasers[i].velocity.y * dt;
                    lasers[i].rotation += 720.0f * dt;
                    WrapPosition(&lasers[i].position, 0);

                    lasers[i].lifeTime -= dt;
                    if (lasers[i].lifeTime <= 0) lasers[i].active = false;
                }
            }

            // 5. Update Asteroids
            for (int i = 0; i < MAX_ASTEROIDS; i++) {
                if (asteroids[i].active) {
                    asteroids[i].position.x += asteroids[i].velocity.x * dt;
                    asteroids[i].position.y += asteroids[i].velocity.y * dt;
                    WrapPosition(&asteroids[i].position, asteroids[i].radius);
                }
            }

            // Infinite asteroid spawning
static float asteroidSpawnTimer = 0.0f;

asteroidSpawnTimer -= dt;

int asteroidCount = 0;

// Count active asteroids
for (int i = 0; i < MAX_ASTEROIDS; i++)
{
    if (asteroids[i].active)
        asteroidCount++;
}


// Keep at least 8 asteroids alive
if (asteroidSpawnTimer <= 0 && asteroidCount < 8)
{
    Vector2 spawnPos;

    do
    {
        spawnPos = (Vector2)
        {
            GetRandomValue(0, SCREEN_WIDTH),
            GetRandomValue(0, SCREEN_HEIGHT)
        };

    } while(CheckCollisionCircles(
        spawnPos,
        150,
        player.position,
        player.radius
    ));


    SpawnAsteroid(
        asteroids,
        spawnPos,
        3
    );


    asteroidSpawnTimer = 2.0f;
}

            // 6. Collision Detection: Lasers vs Asteroids
            for (int i = 0; i < MAX_LASERS; i++) {
                if (!lasers[i].active) continue;

                for (int j = 0; j < MAX_ASTEROIDS; j++) {
                    if (!asteroids[j].active) continue;

                    if (CheckCollisionCircles(lasers[i].position, 2, asteroids[j].position, asteroids[j].radius)) {
                        lasers[i].active = false;
                        asteroids[j].active = false;
                        score += (4 - asteroids[j].sizeClass) * 100; // More points for smaller targets

                        // Split into two smaller asteroids if possible
                        if (asteroids[j].sizeClass > 1) {
                           Vector2 hitPos = asteroids[j].position;
int oldSize = asteroids[j].sizeClass;


asteroids[j].active=false;


score += (4-oldSize)*100;

if (score > highScore)
{
    highScore = score;
}

if(oldSize>1)
{
    SpawnAsteroid(
        asteroids,
        hitPos,
        oldSize-1
    );

    SpawnAsteroid(
        asteroids,
        hitPos,
        oldSize-1
    );
}
                        }
                        break;
                    }
                }
            }

            // 7. Collision Detection: Player vs Asteroids
            for (int i = 0; i < MAX_ASTEROIDS; i++) {
                if (!asteroids[i].active) continue;

                if (CheckCollisionCircles(player.position, player.radius - 4.0f, asteroids[i].position, asteroids[i].radius)) {
                    lives--;
                    if (lives <= 0)
{
    gameOver = true;

    if (score > highScore)
    {
        highScore = score;
        SaveHighScore(highScore);
    }
}
 else {
                        // Reset player to center safely
                        player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
                        player.velocity = (Vector2){ 0, 0 };
                    }
                    // Destroy the specific asteroid hit to avoid instant re-death loop
                    asteroids[i].active = false; 
                    break;
                }
            }
        } else {
            // Restart Logic if Game Over
            if (IsKeyPressed(KEY_R)) {
                player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
                player.velocity = (Vector2){ 0, 0 };
                player.rotation = 0;
                score = 0;
                lives = 6;
                gameOver = false;
                
                for (int i = 0; i < MAX_LASERS; i++) lasers[i].active = false;
                for (int i = 0; i < MAX_ASTEROIDS; i++) asteroids[i].active = false;
                for (int i = 0; i < 5; i++) SpawnAsteroid(asteroids, (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) }, 3);
            }
        }

        // ---------------------------------------------------------------------
        // RENDER SCREEN
        // ---------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        if (!gameOver) {
            // Draw rotating red Shurikens
for (int i = 0; i < MAX_LASERS; i++)
{
    if (lasers[i].active)
    {
        // Larger shuriken size
        float shurikenSize = 25.0f;

        // Outer red blade
        DrawPoly(
            lasers[i].position,
            4,
            shurikenSize,
            lasers[i].rotation,
            RED
        );

        // Inner darker center
        DrawCircleV(
            lasers[i].position,
            4,
            MAROON
        );

        // Small highlight
        DrawCircleV(
            lasers[i].position,
            2,
            ORANGE
        );
    }
}

            // ---------------------------------------------------------------------
// DRAW SHIP (Corrected Counter-Clockwise Winding Order)
// ---------------------------------------------------------------------
// Tip of the ship (Nose)
// Draw circular spaceship
// Ship body
DrawCircleV(
    player.position,
    player.radius,
    RAYWHITE
);


// Reactor glow
DrawCircleV(
    player.position,
    player.radius * 0.35f,
    BLUE
);


// Direction indicator
Vector2 nose =
{
    player.position.x +
    cosf((player.rotation-90)*DEG2RAD)
    * player.radius,

    player.position.y +
    sinf((player.rotation-90)*DEG2RAD)
    * player.radius
};


DrawLineEx(
    player.position,
    nose,
    3,
    RED
);


// Engine flame
if(IsKeyDown(KEY_UP))
{
    Vector2 flame =
    {
        player.position.x -
        cosf((player.rotation-90)*DEG2RAD)
        * player.radius,

        player.position.y -
        sinf((player.rotation-90)*DEG2RAD)
        * player.radius
    };


    DrawCircleV(
        flame,
        player.radius*0.5f,
        ORANGE
    );
}

            // Draw fishes
            for (int i = 0; i < MAX_ASTEROIDS; i++) {
                if (asteroids[i].active) {
                    Vector2 pos = asteroids[i].position;
float r = asteroids[i].radius;


// Fish body
DrawEllipse(
    (int)pos.x,
    (int)pos.y,
    (int)(r * 1.3f),
    (int)r,
    LIGHTGRAY
);


// Tail
Vector2 tail1 = {
    pos.x - r * 1.3f,
    pos.y
};

Vector2 tail2 = {
    pos.x - r * 2.0f,
    pos.y - r * 0.8f
};

Vector2 tail3 = {
    pos.x - r * 2.0f,
    pos.y + r * 0.8f
};

DrawTriangle(
    tail1,
    tail2,
    tail3,
    LIGHTGRAY
);


// Eye
DrawCircle(
    (int)(pos.x + r * 0.7f),
    (int)(pos.y - r * 0.3f),
    r * 0.15f,
    BLACK
);
                }
            }

            // Draw HUD
           DrawText(TextFormat("SCORE: %05i", score), 20, 20, 20, GREEN);
           DrawText(TextFormat("HIGH SCORE: %05i", highScore), 20, 50, 20, YELLOW);
           DrawText(TextFormat("LIVES: %i", lives), 20, 80, 20, RED);
        } 
        else {
            // Draw Game Over Screen
            int textWidth1 = MeasureText("GAME OVER", 40);
            int textWidth2 = MeasureText("Press [R] to Restart", 20);
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - textWidth1 / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);
            DrawText("Press [R] to Restart", SCREEN_WIDTH / 2 - textWidth2 / 2, SCREEN_HEIGHT / 2 + 20, 20, RAYWHITE);
            DrawText(TextFormat("FINAL SCORE: %i", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("FINAL SCORE: %i", score), 20) / 2, SCREEN_HEIGHT / 2 + 60, 20, GREEN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}