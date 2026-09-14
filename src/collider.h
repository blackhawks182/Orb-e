#ifndef collider_h
#define collider_h

#include "raylib.h"

#include "raymath.h"

#include "resource_dir.h"    // utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

#define colliderOrb_size 25.0f

#define enemy_size 25.0f

#define start

#define maxSpeed 500.0f

#define minSpeed 100.0f

#define speedSpeed 500.0f

#define jumpSpeed 500.0f

#define gravity 1000.0f

#define startSpeed 100.0f

#define startMass 1.0f

#define start_X 200.0f

#define start_Y SCREEN_HEIGHT*2/3 - colliderOrb_size

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

void startCollider(void);

#endif