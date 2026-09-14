#ifndef underwater_escape_h
#define underwater_escape_h
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
#define HIGHSCORE_FILE "underwater_highscore.txt"

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

void WrapPosition(Vector2 *pos, float margin);

void SpawnHazard(Hazard *hazards, Vector2 pos);

int LoadHighScore(void);

void SaveHighScore(int score);

void startUnderwaterEscape(void);

#endif