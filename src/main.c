#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

#define CANNON_POSITION 120

#define thetaSpeed 200.0f

typedef enum GameState {
	GAME_STATE_TITLE,
	GAME_STATE_MAIN_MENU
} GameState;

void ResetTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
bool IsTitleScreenFinished(void);

// Runs the game loop and switches between game states.
int main (){

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ORB-E");
	SetTargetFPS(60);
    Rectangle ground = {0,SCREEN_HEIGHT*2/3, SCREEN_WIDTH, SCREEN_HEIGHT/3};
	Rectangle cannonBase = {120, (SCREEN_HEIGHT*2/3) - 15, 100, 15};
	Rectangle cannonBody = {120, (SCREEN_HEIGHT*2/3) - 50, 100, 35};
	Vector2 cannonRotationPoint = (Vector2){ 0.0f, 0.0f};
    int end_projectile_game = 0;
	float rotation = 0.0f;
	while (!WindowShouldClose())
	{
        float deltaTime = GetFrameTime();

		if (IsKeyDown(KEY_UP) && rotation > -70) rotation -= thetaSpeed*deltaTime;
		if (IsKeyDown(KEY_DOWN) && rotation < -1) rotation += thetaSpeed*deltaTime;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);
		DrawRectangleRec(cannonBase, BROWN);
		DrawRectanglePro(cannonBody, cannonRotationPoint, rotation, LIGHTGRAY);
		EndDrawing();

	}


	CloseWindow();
	return 0;
}