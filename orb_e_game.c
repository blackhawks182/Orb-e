#include "orb_e_game.h"
#include "audio.h"
#include "save_load.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define MAX_LASERS 20
#define MAX_ASTEROIDS 100
#define SHIP_SIZE 20.0f
#define LASER_SPEED 700.0f
#define LASER_LIFETIME 1.5f

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
    int sizeClass;
    bool active;
} Asteroid;

static void WrapPosition(Vector2 *pos, float margin) {
    if (pos->x < -margin) pos->x = SCREEN_WIDTH + margin;
    if (pos->x > SCREEN_WIDTH + margin) pos->x = -margin;
    if (pos->y < -margin) pos->y = SCREEN_HEIGHT + margin;
    if (pos->y > SCREEN_HEIGHT + margin) pos->y = -margin;
}

static void SpawnAsteroid(Asteroid *asteroids, Vector2 pos, int sizeClass) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) {
            asteroids[i].position = pos;

            float angle = GetRandomValue(0, 360) * DEG2RAD;
            float speed = GetRandomValue(80, 150) / (float)sizeClass;
            asteroids[i].velocity = (Vector2){ cosf(angle) * speed, sinf(angle) * speed };

            asteroids[i].sizeClass = sizeClass;
            asteroids[i].radius = sizeClass * 15.0f;
            asteroids[i].active = true;
            break;
        }
    }
}

static void ResetGameState(Ship *player, Laser *lasers, Asteroid *asteroids, int *score, int *lives, bool *gameOver) {
    player->position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player->velocity = (Vector2){ 0, 0 };
    player->rotation = 0;
    player->radius = SHIP_SIZE;

    *score = 0;
    *lives = 6;
    *gameOver = false;

    for (int i = 0; i < MAX_LASERS; i++) {
        lasers[i].active = false;
    }

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].active = false;
    }

    for (int i = 0; i < 5; i++) {
        Vector2 spawnPos;
        do {
            spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
        } while (CheckCollisionCircles(spawnPos, 100, player->position, player->radius));

        SpawnAsteroid(asteroids, spawnPos, 3);
    }
}

void RunOrbEGame(void) {
    srand(time(NULL));
    InitAudioDevice();
    SetMasterVolume(0.85f);
    if (!IsAudioDeviceReady()) {
        TraceLog(LOG_WARNING, "Audio device not ready; sound playback will be muted.");
    }
    SetTargetFPS(60);

    typedef enum GameScreen {
        SCREEN_MENU,
        SCREEN_GAME_OPTIONS,
        SCREEN_LEVEL_OPTIONS,
        SCREEN_PLAY,
        SCREEN_GAME_OVER
    } GameScreen;

    Music menuMusic = LoadMusicStream("examples/audio/resources/country.mp3");
    Music levelOneMusic = LoadMusicStream("examples/audio/resources/country.mp3");
    Music levelTwoMusic = LoadMusicStream("examples/audio/resources/mini1111.xm");
    Music levelThreeMusic = LoadMusicStream("examples/audio/resources/target.ogg");

    Sound laserSound = LoadSound("examples/audio/resources/sound.wav");
    Sound asteroidHitSound = LoadSound("examples/audio/resources/coin.wav");
    Sound playerHitSound = LoadSound("examples/audio/resources/weird.wav");
    Sound menuSelectSound = LoadSound("examples/audio/resources/spring.wav");

    SetMusicVolume(menuMusic, 0.35f);
    SetMusicVolume(levelOneMusic, 0.35f);
    SetMusicVolume(levelTwoMusic, 0.35f);
    SetMusicVolume(levelThreeMusic, 0.35f);
    SetSoundVolume(laserSound, 0.25f);
    SetSoundVolume(asteroidHitSound, 0.30f);
    SetSoundVolume(playerHitSound, 0.55f);
    SetSoundVolume(menuSelectSound, 0.45f);

    Ship player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = SHIP_SIZE;

    Laser lasers[MAX_LASERS] = {0};
    Asteroid asteroids[MAX_ASTEROIDS] = {0};

    int score = 0;
    int highScore = LoadHighScore();
    int lives = 6;
    int currentLevel = 1;
    bool gameOver = false;
    GameScreen currentScreen = SCREEN_MENU;

    Music musicTracks[4] = { menuMusic, levelOneMusic, levelTwoMusic, levelThreeMusic };
    int currentTrack = -1;

    Rectangle quickGameButton = { SCREEN_WIDTH / 2.0f - 150.0f, 300.0f, 300.0f, 60.0f };
    Rectangle quitMenuButton = { SCREEN_WIDTH / 2.0f - 150.0f, 390.0f, 300.0f, 60.0f };

    Rectangle gameOptionOne = { SCREEN_WIDTH / 2.0f - 180.0f, 220.0f, 360.0f, 60.0f };
    Rectangle gameOptionTwo = { SCREEN_WIDTH / 2.0f - 180.0f, 300.0f, 360.0f, 60.0f };
    Rectangle gameOptionThree = { SCREEN_WIDTH / 2.0f - 180.0f, 380.0f, 360.0f, 60.0f };
    Rectangle gameOptionsBack = { SCREEN_WIDTH / 2.0f - 150.0f, 490.0f, 300.0f, 60.0f };

    Rectangle playLevelButton = { SCREEN_WIDTH / 2.0f - 150.0f, 300.0f, 300.0f, 60.0f };
    Rectangle levelQuitButton = { SCREEN_WIDTH / 2.0f - 150.0f, 390.0f, 300.0f, 60.0f };
    Rectangle levelBackButton = { SCREEN_WIDTH / 2.0f - 150.0f, 480.0f, 300.0f, 60.0f };

    Rectangle gameOverRestartButton = { SCREEN_WIDTH / 2.0f - 150.0f, 500.0f, 300.0f, 60.0f };
    Rectangle gameOverExitButton = { SCREEN_WIDTH / 2.0f - 150.0f, 580.0f, 300.0f, 60.0f };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vector2 mouse = GetMousePosition();

        if (currentScreen == SCREEN_MENU) {
            if (currentTrack != 0) {
                StopAllMusic(musicTracks, 4);
                PlayMusicStream(menuMusic);
                currentTrack = 0;
            }
            UpdateMusicStream(menuMusic);

            if (IsKeyPressed(KEY_ENTER) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, quickGameButton))) {
                PlaySound(menuSelectSound);
                currentScreen = SCREEN_GAME_OPTIONS;
            }

            if (IsKeyPressed(KEY_ESCAPE) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, quitMenuButton))) {
                break;
            }
        }
        else if (currentScreen == SCREEN_GAME_OPTIONS) {
            if (IsKeyPressed(KEY_ONE) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, gameOptionOne))) {
                currentLevel = 1;
                currentScreen = SCREEN_LEVEL_OPTIONS;
            }

            if (IsKeyPressed(KEY_ESCAPE) ||
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, gameOptionsBack)) {
                currentScreen = SCREEN_MENU;
            }
        }
        else if (currentScreen == SCREEN_LEVEL_OPTIONS) {
            if (IsKeyPressed(KEY_ENTER) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, playLevelButton))) {
                PlaySound(menuSelectSound);
                ResetGameState(&player, lasers, asteroids, &score, &lives, &gameOver);
                currentScreen = SCREEN_PLAY;
            }

            if (IsKeyPressed(KEY_ESCAPE) ||
                IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, levelBackButton)) {
                currentScreen = SCREEN_GAME_OPTIONS;
            }

            if (IsKeyPressed(KEY_Q) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, levelQuitButton))) {
                break;
            }
        }
        else if (currentScreen == SCREEN_PLAY) {
            Music *activeMusic = &levelOneMusic;
            if (score >= 1500) activeMusic = &levelThreeMusic;
            else if (score >= 500) activeMusic = &levelTwoMusic;

            if (currentTrack != (score >= 1500 ? 3 : score >= 500 ? 2 : 1)) {
                StopAllMusic(musicTracks, 4);
                PlayMusicStream(*activeMusic);
                currentTrack = (score >= 1500 ? 3 : score >= 500 ? 2 : 1);
            }
            UpdateMusicStream(*activeMusic);

            if (IsKeyPressed(KEY_E)) {
                currentScreen = SCREEN_LEVEL_OPTIONS;
            }

            if (!gameOver) {
                if (IsKeyDown(KEY_LEFT)) player.rotation -= 250.0f * dt;
                if (IsKeyDown(KEY_RIGHT)) player.rotation += 250.0f * dt;

                Vector2 forward = {
                    cosf((player.rotation - 90) * DEG2RAD),
                    sinf((player.rotation - 90) * DEG2RAD)
                };

                if (IsKeyDown(KEY_UP)) {
                    player.velocity.x += forward.x * 400.0f * dt;
                    player.velocity.y += forward.y * 400.0f * dt;
                }

                if (IsKeyDown(KEY_DOWN)) {
                    player.velocity.x -= forward.x * 200.0f * dt;
                    player.velocity.y -= forward.y * 200.0f * dt;
                }

                player.velocity.x *= 0.99f;
                player.velocity.y *= 0.99f;

                player.position.x += player.velocity.x * dt;
                player.position.y += player.velocity.y * dt;
                WrapPosition(&player.position, player.radius);

                if (IsKeyPressed(KEY_SPACE)) {
                    for (int i = 0; i < MAX_LASERS; i++) {
                        if (!lasers[i].active) {
                            lasers[i].position = Vector2Add(player.position, Vector2Scale(forward, player.radius));
                            lasers[i].velocity = Vector2Scale(forward, LASER_SPEED);
                            lasers[i].lifeTime = LASER_LIFETIME;
                            lasers[i].rotation = 0;
                            lasers[i].active = true;
                            PlaySound(laserSound);
                            break;
                        }
                    }
                }

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

                for (int i = 0; i < MAX_ASTEROIDS; i++) {
                    if (asteroids[i].active) {
                        asteroids[i].position.x += asteroids[i].velocity.x * dt;
                        asteroids[i].position.y += asteroids[i].velocity.y * dt;
                        WrapPosition(&asteroids[i].position, asteroids[i].radius);
                    }
                }

                static float asteroidSpawnTimer = 0.0f;
                asteroidSpawnTimer -= dt;

                int asteroidCount = 0;
                for (int i = 0; i < MAX_ASTEROIDS; i++) {
                    if (asteroids[i].active) asteroidCount++;
                }

                if (asteroidSpawnTimer <= 0 && asteroidCount < 8) {
                    Vector2 spawnPos;
                    do {
                        spawnPos = (Vector2){ GetRandomValue(0, SCREEN_WIDTH), GetRandomValue(0, SCREEN_HEIGHT) };
                    } while (CheckCollisionCircles(spawnPos, 150, player.position, player.radius));

                    SpawnAsteroid(asteroids, spawnPos, 3);
                    asteroidSpawnTimer = 2.0f;
                }

                for (int i = 0; i < MAX_LASERS; i++) {
                    if (!lasers[i].active) continue;

                    for (int j = 0; j < MAX_ASTEROIDS; j++) {
                        if (!asteroids[j].active) continue;

                        if (CheckCollisionCircles(lasers[i].position, 2, asteroids[j].position, asteroids[j].radius)) {
                            lasers[i].active = false;
                            asteroids[j].active = false;
                            score += (4 - asteroids[j].sizeClass) * 100;
                            PlaySound(asteroidHitSound);

                            if (asteroids[j].sizeClass > 1) {
                                Vector2 hitPos = asteroids[j].position;
                                int oldSize = asteroids[j].sizeClass;

                                asteroids[j].active = false;
                                score += (4 - oldSize) * 100;

                                if (score > highScore) {
                                    highScore = score;
                                }

                                if (oldSize > 1) {
                                    SpawnAsteroid(asteroids, hitPos, oldSize - 1);
                                    SpawnAsteroid(asteroids, hitPos, oldSize - 1);
                                }
                            }
                            break;
                        }
                    }
                }

                for (int i = 0; i < MAX_ASTEROIDS; i++) {
                    if (!asteroids[i].active) continue;

                    if (CheckCollisionCircles(player.position, player.radius - 4.0f, asteroids[i].position, asteroids[i].radius)) {
                        lives--;
                        PlaySound(playerHitSound);

                        if (lives <= 0) {
                            gameOver = true;
                            currentScreen = SCREEN_GAME_OVER;
                            if (score > highScore) {
                                highScore = score;
                                SaveHighScore(highScore);
                            }
                        } else {
                            player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
                            player.velocity = (Vector2){ 0, 0 };
                        }

                        asteroids[i].active = false;
                        break;
                    }
                }
            }
            else {
                if (IsKeyPressed(KEY_R)) {
                    PlaySound(menuSelectSound);
                    ResetGameState(&player, lasers, asteroids, &score, &lives, &gameOver);
                    currentScreen = SCREEN_PLAY;
                }
            }
        }
        else if (currentScreen == SCREEN_GAME_OVER) {
            if (IsKeyPressed(KEY_R)) {
                PlaySound(menuSelectSound);
                ResetGameState(&player, lasers, asteroids, &score, &lives, &gameOver);
                currentScreen = SCREEN_PLAY;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, gameOverRestartButton)) {
                PlaySound(menuSelectSound);
                ResetGameState(&player, lasers, asteroids, &score, &lives, &gameOver);
                currentScreen = SCREEN_PLAY;
            }

            if (IsKeyPressed(KEY_ESCAPE) ||
                (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, gameOverExitButton))) {
                currentScreen = SCREEN_LEVEL_OPTIONS;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (currentScreen == SCREEN_MENU) {
            int titleWidth = MeasureText("ORB-E", 60);
            Vector2 mousePos = GetMousePosition();
            Color quickColor = CheckCollisionPointRec(mousePos, quickGameButton) ? LIME : GREEN;
            Color quitColor = CheckCollisionPointRec(mousePos, quitMenuButton) ? LIGHTGRAY : GRAY;

            DrawText("ORB-E", SCREEN_WIDTH / 2 - titleWidth / 2, 140, 60, RAYWHITE);
            DrawRectangleRec(quickGameButton, quickColor);
            DrawRectangleLinesEx(quickGameButton, 2, RAYWHITE);
            DrawText("ENTER", SCREEN_WIDTH / 2 - MeasureText("ENTER", 30) / 2, 315, 30, BLACK);

            DrawRectangleRec(quitMenuButton, quitColor);
            DrawRectangleLinesEx(quitMenuButton, 2, RAYWHITE);
            DrawText("EXIT", SCREEN_WIDTH / 2 - MeasureText("EXIT", 30) / 2, 405, 30, BLACK);
            DrawText("Press Enter or click ENTER", SCREEN_WIDTH / 2 - MeasureText("Press Enter or click ENTER", 20) / 2, 520, 20, LIGHTGRAY);
        }
        else if (currentScreen == SCREEN_GAME_OPTIONS) {
            Vector2 mousePos = GetMousePosition();
            Color opt1Color = CheckCollisionPointRec(mousePos, gameOptionOne) ? LIME : GREEN;
            Color opt2Color = CheckCollisionPointRec(mousePos, gameOptionTwo) ? GRAY : DARKGRAY;
            Color opt3Color = CheckCollisionPointRec(mousePos, gameOptionThree) ? GRAY : DARKGRAY;
            Color backColor = CheckCollisionPointRec(mousePos, gameOptionsBack) ? LIGHTGRAY : GRAY;

            DrawText("SELECT GAME", SCREEN_WIDTH / 2 - MeasureText("SELECT GAME", 48) / 2, 110, 48, RAYWHITE);

            DrawRectangleRec(gameOptionOne, opt1Color);
            DrawRectangleLinesEx(gameOptionOne, 2, RAYWHITE);
            DrawText("ASTEROIDS CLONE", SCREEN_WIDTH / 2 - MeasureText("ASTEROIDS CLONE", 28) / 2, 235, 28, BLACK);

            DrawRectangleRec(gameOptionTwo, opt2Color);
            DrawRectangleLinesEx(gameOptionTwo, 2, GRAY);
            DrawText("COMING SOON", SCREEN_WIDTH / 2 - MeasureText("COMING SOON", 24) / 2, 315, 24, LIGHTGRAY);

            DrawRectangleRec(gameOptionThree, opt3Color);
            DrawRectangleLinesEx(gameOptionThree, 2, GRAY);
            DrawText("COMING SOON", SCREEN_WIDTH / 2 - MeasureText("COMING SOON", 24) / 2, 395, 24, LIGHTGRAY);

            DrawRectangleRec(gameOptionsBack, backColor);
            DrawRectangleLinesEx(gameOptionsBack, 2, RAYWHITE);
            DrawText("BACK", SCREEN_WIDTH / 2 - MeasureText("BACK", 30) / 2, 505, 30, BLACK);
        }
        else if (currentScreen == SCREEN_LEVEL_OPTIONS) {
            Vector2 mousePos = GetMousePosition();
            Color playColor = CheckCollisionPointRec(mousePos, playLevelButton) ? LIME : GREEN;
            Color quitColor = CheckCollisionPointRec(mousePos, levelQuitButton) ? LIGHTGRAY : GRAY;
            Color backColor = CheckCollisionPointRec(mousePos, levelBackButton) ? LIGHTGRAY : GRAY;

            DrawText("LEVEL SELECT", SCREEN_WIDTH / 2 - MeasureText("LEVEL SELECT", 48) / 2, 100, 48, RAYWHITE);
            DrawText(TextFormat("CURRENT LEVEL: %i", currentLevel), SCREEN_WIDTH / 2 - MeasureText(TextFormat("CURRENT LEVEL: %i", currentLevel), 28) / 2, 180, 28, GREEN);
            DrawText(TextFormat("CURRENT SCORE: %i", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("CURRENT SCORE: %i", score), 28) / 2, 220, 28, YELLOW);

            DrawRectangleRec(playLevelButton, playColor);
            DrawRectangleLinesEx(playLevelButton, 2, RAYWHITE);
            DrawText("START GAME", SCREEN_WIDTH / 2 - MeasureText("START GAME", 30) / 2, 315, 30, BLACK);

            DrawRectangleRec(levelQuitButton, quitColor);
            DrawRectangleLinesEx(levelQuitButton, 2, RAYWHITE);
            DrawText("QUIT", SCREEN_WIDTH / 2 - MeasureText("QUIT", 30) / 2, 405, 30, BLACK);

            DrawRectangleRec(levelBackButton, backColor);
            DrawRectangleLinesEx(levelBackButton, 2, RAYWHITE);
            DrawText("BACK", SCREEN_WIDTH / 2 - MeasureText("BACK", 30) / 2, 495, 30, BLACK);
        }
        else if (currentScreen == SCREEN_PLAY) {
            if (!gameOver) {
                for (int i = 0; i < MAX_LASERS; i++) {
                    if (lasers[i].active) {
                        float shurikenSize = 25.0f;
                        DrawPoly(lasers[i].position, 4, shurikenSize, lasers[i].rotation, RED);
                        DrawCircleV(lasers[i].position, 4, MAROON);
                        DrawCircleV(lasers[i].position, 2, ORANGE);
                    }
                }

                DrawCircleV(player.position, player.radius, RAYWHITE);
                DrawCircleV(player.position, player.radius * 0.35f, BLUE);

                Vector2 nose = {
                    player.position.x + cosf((player.rotation - 90) * DEG2RAD) * player.radius,
                    player.position.y + sinf((player.rotation - 90) * DEG2RAD) * player.radius
                };
                DrawLineEx(player.position, nose, 3, RED);

                if (IsKeyDown(KEY_UP)) {
                    Vector2 flame = {
                        player.position.x - cosf((player.rotation - 90) * DEG2RAD) * player.radius,
                        player.position.y - sinf((player.rotation - 90) * DEG2RAD) * player.radius
                    };
                    DrawCircleV(flame, player.radius * 0.5f, ORANGE);
                }

                for (int i = 0; i < MAX_ASTEROIDS; i++) {
                    if (asteroids[i].active) {
                        Vector2 pos = asteroids[i].position;
                        float r = asteroids[i].radius;

                        DrawEllipse((int)pos.x, (int)pos.y, (int)(r * 1.3f), (int)r, LIGHTGRAY);

                        Vector2 tail1 = { pos.x - r * 1.3f, pos.y };
                        Vector2 tail2 = { pos.x - r * 2.0f, pos.y - r * 0.8f };
                        Vector2 tail3 = { pos.x - r * 2.0f, pos.y + r * 0.8f };

                        DrawTriangle(tail1, tail2, tail3, LIGHTGRAY);
                        DrawCircle((int)(pos.x + r * 0.7f), (int)(pos.y - r * 0.3f), r * 0.15f, BLACK);
                    }
                }

                DrawText(TextFormat("SCORE: %05i", score), 20, 20, 20, GREEN);
                DrawText(TextFormat("HIGH SCORE: %05i", highScore), 20, 50, 20, YELLOW);
                DrawText(TextFormat("LIVES: %i", lives), 20, 80, 20, RED);
            }
        }
        else if (currentScreen == SCREEN_GAME_OVER) {
            int textWidth1 = MeasureText("GAME OVER", 40);
            int textWidth2 = MeasureText("Press [R] to Restart", 20);
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - textWidth1 / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);
            DrawText("Press [R] to Restart", SCREEN_WIDTH / 2 - textWidth2 / 2, SCREEN_HEIGHT / 2 + 20, 20, RAYWHITE);
            DrawText(TextFormat("FINAL SCORE: %i", score), SCREEN_WIDTH / 2 - MeasureText(TextFormat("FINAL SCORE: %i", score), 20) / 2, SCREEN_HEIGHT / 2 + 60, 20, GREEN);

            DrawRectangleRec(gameOverRestartButton, GREEN);
            DrawRectangleLinesEx(gameOverRestartButton, 2, RAYWHITE);
            DrawText("RESTART", SCREEN_WIDTH / 2 - MeasureText("RESTART", 30) / 2, 515, 30, BLACK);

            DrawRectangleRec(gameOverExitButton, LIGHTGRAY);
            DrawRectangleLinesEx(gameOverExitButton, 2, RAYWHITE);
            DrawText("EXIT", SCREEN_WIDTH / 2 - MeasureText("EXIT", 30) / 2, 595, 30, BLACK);
        }

        EndDrawing();
    }

    StopAllMusic(musicTracks, 4);
    UnloadMusicStream(menuMusic);
    UnloadMusicStream(levelOneMusic);
    UnloadMusicStream(levelTwoMusic);
    UnloadMusicStream(levelThreeMusic);
    UnloadSound(laserSound);
    UnloadSound(asteroidHitSound);
    UnloadSound(playerHitSound);
    UnloadSound(menuSelectSound);
    CloseAudioDevice();
    return;
}
