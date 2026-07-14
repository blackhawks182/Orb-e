#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

typedef enum GameState {
	GAME_STATE_TITLE,
	GAME_STATE_MAIN_MENU
} GameState;

void ResetTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
bool IsTitleScreenFinished(void);
void ResetMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
bool IsMainMenuQuitRequested(void);

// Runs the game loop and switches between game states.
int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ORB-E");

	GameState gameState = GAME_STATE_TITLE;
	ResetTitleScreen();
	
	while (!WindowShouldClose())
	{
		if (gameState == GAME_STATE_TITLE)
		{
			UpdateTitleScreen();

			if (IsTitleScreenFinished())
			{
				gameState = GAME_STATE_MAIN_MENU;
				ResetMainMenu();
			}
		}
		else if (gameState == GAME_STATE_MAIN_MENU)
		{
			UpdateMainMenu();

			if (IsMainMenuQuitRequested())
			{
				CloseWindow();
				break;
			}
		}

		BeginDrawing();

		if (gameState == GAME_STATE_TITLE)
		{
			DrawTitleScreen();
		}
		else if (gameState == GAME_STATE_MAIN_MENU)
		{
			DrawMainMenu();
		}
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}