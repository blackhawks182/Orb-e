#include "raylib.h"

typedef enum GameState
{
    GAME_STATE_TITLE,
    GAME_STATE_MAIN_MENU,
    GAME_STATE_GAME_SELECTION
} GameState;

void ResetTitleScreen(void);
void UpdateTitleScreen(void);
void DrawTitleScreen(void);
bool IsTitleScreenFinished(void);

void ResetMainMenu(void);
void UpdateMainMenu(void);
void DrawMainMenu(void);
bool IsMainMenuQuitRequested(void);
bool IsMainMenuStartGameRequested(void);

void ResetGameSelection(void);
void UpdateGameSelection(void);
void DrawGameSelection(void);
bool IsGameSelectionReturnRequested(void);
bool IsGameSelectionQuitRequested(void);

void LoadMenuFont(void);
void UnloadMenuFont(void);

void LoadMenuBackground(void);
void UnloadMenuBackground(void);

void LoadTitleBackground(void);
void UnloadTitleBackground(void);

void LoadMenuMusic(void);
void StartMenuMusic(void);
void UpdateMenuMusic(void);
void StopMenuMusic(void);
void UnloadMenuMusic(void);

static void CloseMenuAndExit(void)
{
    StopMenuMusic();
    UnloadMenuMusic();
    UnloadMenuBackground();
    UnloadTitleBackground();
    UnloadMenuFont();
    CloseAudioDevice();
    CloseWindow();
}

// Runs the game loop and switches between game states.
int main()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    SetTargetFPS(60);

    InitWindow(1200, 800, "ORB-E");
    InitAudioDevice();

    LoadMenuFont();
    LoadMenuBackground();
    LoadTitleBackground();
    LoadMenuMusic();

    GameState gameState = GAME_STATE_TITLE;

    ResetTitleScreen();

    while (!WindowShouldClose())
    {
        UpdateMenuMusic();

        switch (gameState)
        {
        case GAME_STATE_TITLE:
            UpdateTitleScreen();

            if (IsTitleScreenFinished())
            {
                gameState = GAME_STATE_MAIN_MENU;
                ResetMainMenu();
                StartMenuMusic();
            }
            break;

        case GAME_STATE_MAIN_MENU:
            UpdateMainMenu();

            if (IsMainMenuStartGameRequested())
            {
                gameState = GAME_STATE_GAME_SELECTION;
                ResetGameSelection();
            }

            if (IsMainMenuQuitRequested())
            {
                CloseMenuAndExit();
                return 0;
            }
            break;

        case GAME_STATE_GAME_SELECTION:
            UpdateGameSelection();

            if (IsGameSelectionQuitRequested())
            {
                CloseMenuAndExit();
                return 0;
            }

            if (IsGameSelectionReturnRequested())
            {
                gameState = GAME_STATE_MAIN_MENU;
                ResetMainMenu();
                StartMenuMusic();
            }
            break;
        }

        BeginDrawing();

        switch (gameState)
        {
        case GAME_STATE_TITLE:
            DrawTitleScreen();
            break;

        case GAME_STATE_MAIN_MENU:
            DrawMainMenu();
            break;

        case GAME_STATE_GAME_SELECTION:
            DrawGameSelection();
            break;
        }

        EndDrawing();
    }

    if (IsWindowReady())
    {
        CloseMenuAndExit();
    }

    return 0;
}