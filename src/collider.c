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
}
