#include "raylib.h"

#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

#define CANNON_POSITION 120

#define thetaSpeed 200.0f

#define projectileOrb_size 15.0f

#define maxSpeed 300.0f

#define speedSpeed 200.0f

#define minSpeed 0.0f

typedef enum GameState {
	GAME_STATE_TITLE,
	GAME_STATE_MAIN_MENU
} GameState;

void ResetTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
bool IsTitleScreenFinished(void);



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
int main (){

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ORB-E");
	SetTargetFPS(60);

    typedef struct projectileOrb{
        Vector2 position;
        Vector2 velocity;
        int inAir;
    } projectileOrb;
    
    Rectangle ground = {0,SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
	Rectangle cannonBase = {120, (SCREEN_HEIGHT*2/3) - 15, 100, 15};
	Rectangle cannonBody = {120, (SCREEN_HEIGHT*2/3) - 50, 100, 35};
    Rectangle speedBar = {120, (SCREEN_HEIGHT*2/3) - 180, 100, 5};
	Vector2 cannonRotationPoint = (Vector2){ 0.0f, 0.0f};
    int end_projectile_game = 0;
    int orbInCannon = 1;
    int orbInAir = 0;
    int barIncreasing = 1;
	float rotation = 0.0f;
    float launchSpeed = 0.0f;
    projectileOrb Orb;
    Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
	while (!WindowShouldClose())
	{
        float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_UP) && rotation > -70) rotation -= thetaSpeed*deltaTime;
		if (IsKeyDown(KEY_DOWN) && rotation < -1) rotation += thetaSpeed*deltaTime;
        if (launchSpeed >= maxSpeed) barIncreasing = 0;
        if (launchSpeed <= minSpeed) barIncreasing = 1;
        if (IsKeyDown(KEY_Z) && orbInCannon) 
            if (launchSpeed <= maxSpeed && barIncreasing) launchSpeed += speedSpeed*deltaTime;
            else if (launchSpeed >= minSpeed && !barIncreasing) launchSpeed -= speedSpeed*deltaTime;
        speedBar.width = (100*launchSpeed/maxSpeed);
        Orb.position = rightEdgeMidpoint(cannonBody, cannonRotationPoint, rotation);
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);
        DrawCircleV(Orb.position, projectileOrb_size, RED);
		DrawRectangleRec(cannonBase, BROWN);
        DrawRectangleRec(speedBar, GREEN);
		DrawRectanglePro(cannonBody, cannonRotationPoint, rotation, LIGHTGRAY);
		EndDrawing();

	}


	CloseWindow();
	return 0;
}