#include "raylib.h"

typedef enum GameState {
	GAME_STATE_TITLE,
	GAME_STATE_MAIN_MENU
} GameState;

void ResetTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
bool IsTitleScreenFinished(void);

// Runs the game loop and switches between game states.
int main ()
{
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	InitWindow(1200, 800, "ORB-E");

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
			}
		}

		BeginDrawing();

		if (gameState == GAME_STATE_TITLE)
		{
			DrawTitleScreen();
		}
		else if (gameState == GAME_STATE_MAIN_MENU)
		{
			// TODO: Draw the Main Menu when that feature is implemented.
			ClearBackground(WHITE);
		}
		
		EndDrawing();
	}

	CloseWindow();
	return 0;
}