#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define ORB_SIZE 20.0f
#define MOVE_SPEED 350.0f // Speed of direct movement in pixels per second

#define MAX_SHURIKENS 20
#define SHURIKEN_SPEED 700.0f
#define SHURIKEN_LIFETIME 1.5f // Seconds before a shuriken disappears
#define MAX_HAZARDS 100
#define MAX_LIVES 3
#define INVULNERABILITY_TIME 2.0f // Invulnerability duration after getting hit
#define HIGHSCORE_FILE "highscore.txt"

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

typedef struct Hazard {
    Vector2 position;
    Vector2 velocity;
    float radius;
    bool active;
} Hazard;

// Helper to wrap positions around screen edges
void WrapPosition(Vector2 *pos, float margin) {
    if (pos->x < -margin) pos->x = SCREEN_WIDTH + margin;
    if (pos->x > SCREEN_WIDTH + margin) pos->x = -margin;
    if (pos->y < -margin) pos->y = SCREEN_HEIGHT + margin;
    if (pos->y > SCREEN_HEIGHT + margin) pos->y = -margin;
}

// Helper to spawn a single sphere hazard
void SpawnHazard(Hazard *hazards, Vector2 pos) {
    for (int i = 0; i < MAX_HAZARDS; i++) {
        if (!hazards[i].active) {
            hazards[i].position = pos;
            
            float angle = GetRandomValue(0, 360) * DEG2RAD;
            float speed = (float)GetRandomValue(80, 150); 
            hazards[i].velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };
            
            hazards[i].radius = 45.0f; // Fixed size (3 * 15.0f)
            hazards[i].active = true;
            break;
        }
    }
}

// Helper to load high score from file
int LoadHighScore(void) {
    if (FileExists(HIGHSCORE_FILE)) {
        char *fileText = LoadFileText(HIGHSCORE_FILE);
        if (fileText != NULL) {
            int score = atoi(fileText);
            UnloadFileText(fileText);
            return score;
        }
    }
    return 0;
}

// Helper to save high score to file
void SaveHighScore(int score) {
    char scoreStr[16];
    snprintf(scoreStr, sizeof(scoreStr), "%d", score);
    SaveFileText(HIGHSCORE_FILE, scoreStr);
}


void startUnderwaterEscape(void) {
    // 1. Initialization
    srand(time(NULL)); // Seed the random number generator
    Orb player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = ORB_SIZE;
    player.rotation = 0.0f; // Starts facing up

    Shuriken shurikens[MAX_SHURIKENS] = {0};
    Hazard hazards[MAX_HAZARDS] = {0};
    int highScore = LoadHighScore(); // Read persisted high score from file
    int lives = MAX_LIVES;
    float invulnerableTimer = 0.0f; 
    int score = 0;
    bool gameOver = false;

     // Spawn 6 initial large hazards at a safe distance from player center (150px buffer)
    for (int i = 0; i < 6; i++) {
        Vector2 spawnPos;
        do {
            spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
        } while (CheckCollisionCircles(spawnPos, 150.0f, player.position, player.radius));
        
        SpawnHazard(hazards, spawnPos);
    }
    
    Texture2D backgroundTexture = LoadTexture("assets/title_background.png");

    // 2. Main Game Loop
    while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO)) {
		float dt = GetFrameTime();

               if (!gameOver) {
            // Update invulnerability timer
            if (invulnerableTimer > 0.0f) {
                invulnerableTimer -= dt;
            }

            // UPDATE MOVEMENT 
            Vector2 movement = { 0.0f, 0.0f };

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  movement.x -= 1.0f;
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) movement.x += 1.0f;
            if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    movement.y -= 1.0f;
            if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  movement.y += 1.0f;

            if (movement.x != 0.0f || movement.y != 0.0f) {
                float length = sqrtf(movement.x * movement.x + movement.y * movement.y);
                movement.x /= length;
                movement.y /= length;

                player.position.x += movement.x * MOVE_SPEED * dt;
                player.position.y += movement.y * MOVE_SPEED * dt;

                player.rotation = (atan2f(movement.y, movement.x) * RAD2DEG) + 90.0f;
            }

            // Screen edge wrapping
            WrapPosition(&player.position, player.radius);

            // Compute forward facing vector
            Vector2 forward = { 
                cosf((player.rotation - 90.0f) * DEG2RAD), 
                sinf((player.rotation - 90.0f) * DEG2RAD) 
            };

            // FIRING SHURIKENS
            if (IsKeyPressed(KEY_SPACE)) {
                for (int i = 0; i < MAX_SHURIKENS; i++) {
                    if (!shurikens[i].active) {
                        shurikens[i].position = (Vector2){
                            player.position.x + forward.x * player.radius,
                            player.position.y + forward.y * player.radius
                        };
                        
                        shurikens[i].velocity = (Vector2){
                            forward.x * SHURIKEN_SPEED,
                            forward.y * SHURIKEN_SPEED
                        };
                        
                        shurikens[i].lifeTime = SHURIKEN_LIFETIME;
                        shurikens[i].rotation = 0.0f;
                        shurikens[i].active = true;
                        break; 
                    }
                }
            }

            // UPDATE PROJECTILES
            for (int i = 0; i < MAX_SHURIKENS; i++) {
                if (shurikens[i].active) {
                    shurikens[i].position.x += shurikens[i].velocity.x * dt;
                    shurikens[i].position.y += shurikens[i].velocity.y * dt;
                    shurikens[i].rotation += 720.0f * dt;
                    
                    float margin = 25.0f;
                    if (shurikens[i].position.x < -margin || 
                        shurikens[i].position.x > SCREEN_WIDTH + margin || 
                        shurikens[i].position.y < -margin || 
                        shurikens[i].position.y > SCREEN_HEIGHT + margin) 
                    {
                        shurikens[i].active = false;
                    }

                    shurikens[i].lifeTime -= dt;
                    if (shurikens[i].lifeTime <= 0) {
                        shurikens[i].active = false;
                    }
                }
            }

            // UPDATE HAZARDS
            for (int i = 0; i < MAX_HAZARDS; i++) {
                if (hazards[i].active) {
                    hazards[i].position.x += hazards[i].velocity.x * dt;
                    hazards[i].position.y += hazards[i].velocity.y * dt;
                    WrapPosition(&hazards[i].position, hazards[i].radius);
                }
            }

            // COLLISIONS: SHURIKEN vs HAZARD
            for (int i = 0; i < MAX_SHURIKENS; i++) {
                if (!shurikens[i].active) continue;

                for (int j = 0; j < MAX_HAZARDS; j++) {
                    if (!hazards[j].active) continue;

                    if (CheckCollisionCircles(shurikens[i].position, 12.5f, hazards[j].position, hazards[j].radius)) {
                        shurikens[i].active = false;
                        score += 100;
                        if (score > highScore) {
                            highScore = score;
                            SaveHighScore(highScore);
                        }
                        hazards[j].active = false;
                        break; 
                    }
                }
            }

            // COLLISIONS: PLAYER vs HAZARD
            if (invulnerableTimer <= 0.0f) {
                for (int i = 0; i < MAX_HAZARDS; i++) {
                    if (hazards[i].active) {
                        if (CheckCollisionCircles(player.position, player.radius, hazards[i].position, hazards[i].radius)) {
                            lives--;
                            if (lives <= 0) {
                                gameOver = true;
                            } else {
                                player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
                                invulnerableTimer = INVULNERABILITY_TIME;
                            }
                            break;
                        }
                    }
                }
            }

            // RESPAWN HAZARDS
            int activeHazardCount = 0;
            for (int i = 0; i < MAX_HAZARDS; i++) {
                if (hazards[i].active) activeHazardCount++;
            }

            if (activeHazardCount <= 3) {
                for (int i = 0; i < 3; i++) {
                    Vector2 spawnPos;
                    int attempts = 0;

                    do {
                        spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
                        attempts++;
                    } while (CheckCollisionCircles(spawnPos, 150.0f, player.position, player.radius) && attempts < 100);
                    
                    SpawnHazard(hazards, spawnPos);
                }
            }
        } 
        else {
            // RESTART LOGIC WHEN IN GAME OVER STATE
            if (IsKeyPressed(KEY_R)) {
                player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
                score = 0;
                highScore = LoadHighScore();
                lives = MAX_LIVES;
                invulnerableTimer = 0.0f;
                gameOver = false;

                for (int i = 0; i < MAX_SHURIKENS; i++) shurikens[i].active = false;
                for (int i = 0; i < MAX_HAZARDS; i++) hazards[i].active = false;

                for (int i = 0; i < 6; i++) {
                    Vector2 spawnPos;
                    int attempts = 0;
                    do {
                        spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
                        attempts++;
                    } while (CheckCollisionCircles(spawnPos, 150.0f, player.position, player.radius) && attempts < 100);
                    
                    SpawnHazard(hazards, spawnPos);
                }
            }
        }

        // RENDER
        
       BeginDrawing();
       DrawTexturePro(
            backgroundTexture,
            (Rectangle){0, 0, backgroundTexture.width, backgroundTexture.height},
            (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        if (!gameOver) {
            // Draw Active Shurikens
            for (int i = 0; i < MAX_SHURIKENS; i++) {
                if (shurikens[i].active) {
                    float shurikenSize = 25.0f;
                    DrawPoly(shurikens[i].position, 4, shurikenSize, shurikens[i].rotation, BLACK);
                    DrawCircleV(shurikens[i].position, 4.0f, MAROON);
                    DrawCircleV(shurikens[i].position, 2.0f, ORANGE);
                }
            }

            // Draw Orb Body
            if (invulnerableTimer <= 0.0f || (int)(invulnerableTimer * 10) % 2 == 0) {
                DrawCircleV(player.position, player.radius, RAYWHITE);
                DrawCircleV(player.position, player.radius * 0.35f, GetColor(0xF2BE44FF));

                Vector2 forward = { 
                    cosf((player.rotation - 90.0f) * DEG2RAD), 
                    sinf((player.rotation - 90.0f) * DEG2RAD) 
                };
                Vector2 noseLineEnd = {
                    player.position.x + forward.x * player.radius,
                    player.position.y + forward.y * player.radius
                };
                DrawLineEx(player.position, noseLineEnd, 3.0f, RED);
            }

            // Draw Active Hazards
            for (int i = 0; i < MAX_HAZARDS; i++) {
                if (hazards[i].active) {
                    DrawCircleV(hazards[i].position, hazards[i].radius, GRAY);
                    DrawCircleLines((int)hazards[i].position.x, (int)hazards[i].position.y, hazards[i].radius, LIGHTGRAY);
                }
            }

            // Draw Gameplay UI
            DrawText(TextFormat("SCORE: %05d", score), 20, 20, 20, RAYWHITE);
            DrawText(TextFormat("HIGH SCORE: %05d", highScore), 20, 50, 20, YELLOW);

            DrawText("LIVES:", 20, 80, 20, RAYWHITE);
            for (int i = 0; i < lives; i++) {
                DrawCircle(100 + (i * 25), 90, 8, RAYWHITE);
            }
        } 
        else {
            // Dedicated Game Over Screen (Clean blue screen with text only)
            const char *gameOverText = "GAME OVER";
            const char *scoreText = TextFormat("FINAL SCORE: %05d", score);
            const char *highScoreText = TextFormat("HIGH SCORE: %05d", highScore);
            const char *restartText = "Press 'R' to Restart";

            DrawText(gameOverText, SCREEN_WIDTH / 2 - MeasureText(gameOverText, 50) / 2, SCREEN_HEIGHT / 2 - 80, 50, RED);
            DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, 24) / 2, SCREEN_HEIGHT / 2 - 10, 24, RAYWHITE);
            DrawText(highScoreText, SCREEN_WIDTH / 2 - MeasureText(highScoreText, 24) / 2, SCREEN_HEIGHT / 2 + 25, 24, YELLOW);
            DrawText(restartText, SCREEN_WIDTH / 2 - MeasureText(restartText, 20) / 2, SCREEN_HEIGHT / 2 + 80, 20, LIGHTGRAY);
        }

        EndDrawing();
    }
    UnloadTexture(backgroundTexture);
}