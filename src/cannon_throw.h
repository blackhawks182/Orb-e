#ifndef cannon_throw_h
#define cannon_throw_h

#include "raylib.h"

#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

#define CANNON_POSITION 120

#define thetaSpeed 200.0f

#define projectileOrb_size 15.0f

#define maxSpeed 1000.0f

#define speedSpeed 1666.6f

#define minSpeed 0.0f

#define gravity 600.0f

typedef struct projectileOrb{
    Vector2 position;
    Vector2 velocity;
    int inAir;
} projectileOrb;

typedef struct obstacle{
Rectangle rectangle;
Vector2 velocity;
int type;
int active;
float leftbound;
float rightbound;
} obstacle;

Vector2 rightEdgeMidpoint(Rectangle rect, Vector2 origin, float rotation);

void startCannonThrow(void);

#endif