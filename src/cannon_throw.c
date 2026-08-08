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

    typedef struct obstacle{
    Rectangle rectangle;
    Vector2 velocity;
    int type;
    int active;
    float leftbound;
    float rightbound;
    } obstacle;

    obstacle obstacles[10];

    int obstacleSet = GetRandomValue(0, 2);
    int obstacleCount = 0;

    if (obstacleSet == 0){
        obstacleCount = 2;
        obstacles[0].rectangle = (Rectangle){600, 300, 50, 50};
        obstacles[0].velocity = (Vector2){0, 0};
        obstacles[0].type = 0;
        obstacles[0].active = 1;

        obstacles[1].rectangle = (Rectangle){800, 450, 50, 50};
        obstacles[1].velocity = (Vector2){0, 0};
        obstacles[1].type = 0;
        obstacles[1].active = 1;
    }

    if (obstacleSet == 1){
        obstacleCount = 2;
        obstacles[0].rectangle = (Rectangle){600, 300, 50, 50};
        obstacles[0].velocity = (Vector2){100, 0};
        obstacles[0].type = 1;
        obstacles[0].active = 1;
        obstacles[0].leftbound = 500;
        obstacles[0].rightbound = 800;

        obstacles[1].rectangle = (Rectangle){800, 450, 50, 50};
        obstacles[1].velocity = (Vector2){0, -100};
        obstacles[1].type = 1;
        obstacles[1].active = 1;
    }

    if (obstacleSet == 2){
        obstacleCount = 3;
        obstacles[0].rectangle = (Rectangle){600, 300, 50, 50};
        obstacles[0].velocity = (Vector2){0, 0};
        obstacles[0].type = 0;
        obstacles[0].active = 1;

        obstacles[1].rectangle = (Rectangle){800, 450, 50, 50};
        obstacles[1].velocity = (Vector2){0, 100};
        obstacles[1].type = 1;
        obstacles[1].active = 1;

        obstacles[2].rectangle = (Rectangle){950, 250, 50, 50};
        obstacles[2].velocity = (Vector2){100, 0};
        obstacles[2].type = 1;
        obstacles[2].active = 1;
        obstacles[2].leftbound = 800;
        obstacles[2].rightbound = 1100;
    }

    
    Rectangle ground = {0,SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
	Rectangle cannonBase = {120, (SCREEN_HEIGHT*2/3) - 15, 100, 15};
	Rectangle cannonBody = {120, (SCREEN_HEIGHT*2/3) - 50, 100, 35};
    Rectangle speedBar = {120, (SCREEN_HEIGHT*2/3) - 180, 100, 5};
	Vector2 cannonRotationPoint = (Vector2){0.0f, 0.0f};
    int end_projectile_game = 0;
    int barIncreasing = 1;
	float rotation = 0.0f;
    float launchAngle = 0.0f;
    float launchSpeed = 0.0f;
    projectileOrb Orb;
    Orb.inAir = 0;
    Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
	while (!WindowShouldClose() && !IsKeyPressed(KEY_ZERO))
	{
        float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_UP) && rotation > -70) rotation -= thetaSpeed*deltaTime;
		if (IsKeyDown(KEY_DOWN) && rotation < -1) rotation += thetaSpeed*deltaTime;
        if (launchSpeed >= maxSpeed) barIncreasing = 0;
        if (launchSpeed <= minSpeed) barIncreasing = 1;
        if (IsKeyDown(KEY_Z) && !Orb.inAir) 
            if (launchSpeed <= maxSpeed && barIncreasing) launchSpeed += speedSpeed*deltaTime;
            else if (launchSpeed >= minSpeed && !barIncreasing) launchSpeed -= speedSpeed*deltaTime;
        if (IsKeyReleased(KEY_Z) && !Orb.inAir) {
            Orb.inAir = 1;
            launchAngle = rotation;
            Orb.velocity.x = launchSpeed*cosf(launchAngle*DEG2RAD);
            Orb.velocity.y = launchSpeed*sinf(launchAngle*DEG2RAD) - gravity*deltaTime;
        }
        if (Orb.inAir){
            Orb.velocity.y += gravity*deltaTime;
            Orb.position.x += Orb.velocity.x * deltaTime;
            Orb.position.y += Orb.velocity.y *deltaTime;
            for (int i = 0; i <= obstacleCount-1; i++){
                if (obstacles[i].active && CheckCollisionCircleRec(Orb.position, projectileOrb_size, obstacles[i].rectangle)){
                    obstacles[i].active = 0;
                    Orb.inAir = 0;
                    launchSpeed = 0;
                }
            }

        }
        if (Orb.inAir && Orb.position.x > SCREEN_WIDTH + projectileOrb_size || Orb.position.y > SCREEN_HEIGHT*2/3 - projectileOrb_size){
            Orb.inAir = 0;
            launchSpeed = 0;
        }
        if(!Orb.inAir){
            speedBar.width = (100*launchSpeed/maxSpeed);
            Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
        }

        for (int i = 0; i <= obstacleCount-1; i++){
            if (obstacles[i].active && obstacles[i].type == 1){
                obstacles[i].rectangle.y += obstacles[i].velocity.y * deltaTime;
                obstacles[i].rectangle.x += obstacles[i].velocity.x * deltaTime;
                if (obstacles[i].rectangle.y < 100 || obstacles[i].rectangle.y + obstacles[i].rectangle.height > SCREEN_HEIGHT*2/3){
                    obstacles[i].velocity.y *= -1;
                }
                if (obstacles[i].rectangle.x < obstacles[i].leftbound || obstacles[i].rectangle.x + obstacles[i].rectangle.width > obstacles[i].rightbound){
                    obstacles[i].velocity.x *= -1;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);
        DrawCircleV(Orb.position, projectileOrb_size, RED);
		DrawRectangleRec(cannonBase, BROWN);
        for (int i = 0; i <= obstacleCount-1; i++){
            if (obstacles[i].active){
                DrawRectangleRec(obstacles[i].rectangle, BLUE);
            }
        }
        if(!Orb.inAir)DrawRectangleRec(speedBar, GREEN);
		DrawRectanglePro(cannonBody, cannonRotationPoint, rotation, LIGHTGRAY);
		EndDrawing();

	}
}
