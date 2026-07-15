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

Vector2 rightEdgeMidpoint(Rectangle rect, Vector2 origin, float rotation)
{
    float rotationRadians = rotation * DEG2RAD;
    Vector2 unrotatedRectPoint = { rect.width, rect.height / 2.0f };
    
    Vector2 rotatedRectPoint;
    rotatedRectPoint.x = (unrotatedRectPoint.x * cosf(rotationRadians) - unrotatedRectPoint.y * sinf(rotationRadians)) + rect.x; //xnew = xold * costheta - yold * sintheta
    rotatedRectPoint.y = (unrotatedRectPoint.x * sinf(rotationRadians) + unrotatedRectPoint.y * cosf(rotationRadians)) + rect.y; //ynew = xold * sintheta + yold * costheta
    return rotatedRectPoint;
}


// Runs the game loop and switches between game states.

void startCannonThrow(void){
    typedef struct projectileOrb{
        Vector2 position;
        Vector2 velocity;
        int inAir;
    } projectileOrb;
    
    Rectangle ground = {0,SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
	Rectangle cannonBase = {120, (SCREEN_HEIGHT*2/3) - 15, 100, 15};
	Rectangle cannonBody = {120, (SCREEN_HEIGHT*2/3) - 50, 100, 35};
    Rectangle speedBar = {120, (SCREEN_HEIGHT*2/3) - 180, 100, 5};
	Vector2 cannonRotationPoint = (Vector2){0.0f, 0.0f};
    int end_projectile_game = 0;
    int barIncreasing = 1;
	float rotation = 0.0f;
    float launchAngle - 0.0f;
    float launchSpeed = 0.0f;
    projectileOrb Orb;
    Orb.inAir = 0;
    Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
	while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO))
	{
        float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_UP) && rotation > -70 && !Orb.inAir) rotation -= thetaSpeed*deltaTime;
		if (IsKeyDown(KEY_DOWN) && rotation < -1 && !Orb.inAir) rotation += thetaSpeed*deltaTime;
        if (launchSpeed >= maxSpeed) barIncreasing = 0;
        if (launchSpeed <= minSpeed) barIncreasing = 1;
        if (IsKeyDown(KEY_Z) && !Orb.inAir) 
            if (launchSpeed <= maxSpeed && barIncreasing) launchSpeed += speedSpeed*deltaTime;
            else if (launchSpeed >= minSpeed && !barIncreasing) launchSpeed -= speedSpeed*deltaTime;
        if (IsKeyReleased(KEY_Z) && !Orb.inAir) {
            Orb.inAir = 1;
            Orb.velocity.x = launchSpeed*cosf(rotation*DEG2RAD);
            Orb.velocity.y = launchSpeed*sinf(rotation*DEG2RAD) - gravity*deltaTime;
        }
        if(!Orb.inAir){
            speedBar.width = (100*launchSpeed/maxSpeed);
            Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
        }
        if (Orb.inAir){
            Orb.velocity.y += gravity*deltaTime;
            Orb.position.x += Orb.velocity.x * deltaTime;
            Orb.position.y += Orb.velocity.y *deltaTime;
        }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);
        DrawCircleV(Orb.position, projectileOrb_size, RED);
		DrawRectangleRec(cannonBase, BROWN);
        if(!Orb.inAir)DrawRectangleRec(speedBar, GREEN);
		DrawRectanglePro(cannonBody, cannonRotationPoint, rotation, LIGHTGRAY);
		EndDrawing();

	}
}
