#include "raylib.h"

static float titleTimer;
static bool titleFinished;

static int mainMenuSelection;
static bool mainMenuQuit;
static bool mainMenuStart;

static int gameSelection;
static bool gameSelectionBack;

void startCannonThrow(void);
void startUnderwaterEscape(void);
void startCollider(void);

// No custom font is used in the simple menu.
void LoadMenuFont(void)
{
}

// No custom font needs to be unloaded.
void UnloadMenuFont(void)
{
}

// Reset the title screen timer.
void ResetTitleScreen(void)
{
    titleTimer = 0.0f;
    titleFinished = false;
}

// Update the title screen time.
void UpdateTitleScreen(void)
{
    titleTimer += GetFrameTime();

    if (titleTimer >= 2.0f)
    {
        titleTimer = 2.0f;
        titleFinished = true;
    }
}

// Check if the title screen is done.
bool IsTitleScreenFinished(void)
{
    return titleFinished;
}

// Draw a simple title screen.
void DrawTitleScreen(void)
{
    char *title = "ORB-E";
    int fontSize = 80;
    int textWidth = MeasureText(title, fontSize);
    int x = (GetScreenWidth() - textWidth) / 2;
    int y = (GetScreenHeight() - fontSize) / 2;

    ClearBackground(WHITE);
    DrawText(title, x, y, fontSize, BLACK);
}

// Reset the Main Menu.
void ResetMainMenu(void)
{
    mainMenuSelection = 0;
    mainMenuQuit = false;
    mainMenuStart = false;
}

// Update Main Menu input.
void UpdateMainMenu(void)
{
    int buttonX = (GetScreenWidth() - 420) / 2;
    Rectangle startButton = { buttonX, 350, 420, 82 };
    Rectangle quitButton = { buttonX, 456, 420, 82 };
    Vector2 mouse = GetMousePosition();
    float wheel = GetMouseWheelMove();
    bool clicked = false;

    if (IsKeyPressed(KEY_UP) || wheel > 0.0f)
        mainMenuSelection = 1 - mainMenuSelection;

    if (IsKeyPressed(KEY_DOWN) || wheel < 0.0f)
        mainMenuSelection = 1 - mainMenuSelection;

    if (CheckCollisionPointRec(mouse, startButton))
    {
        mainMenuSelection = 0;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (CheckCollisionPointRec(mouse, quitButton))
    {
        mainMenuSelection = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (IsKeyPressed(KEY_ENTER) || clicked)
    {
        if (mainMenuSelection == 0)
            mainMenuStart = true;
        else
            mainMenuQuit = true;
    }
}

// Check if Quit Game was selected.
bool IsMainMenuQuitRequested(void)
{
    return mainMenuQuit;
}

// Check if Start Game was selected.
bool IsMainMenuStartGameRequested(void)
{
    return mainMenuStart;
}

// Draw the Main Menu.
void DrawMainMenu(void)
{
    char *title = "ORB-E";
    char *startText = "START GAME";
    char *quitText = "QUIT GAME";
    int buttonX = (GetScreenWidth() - 420) / 2;
    int titleX = (GetScreenWidth() - MeasureText(title, 60)) / 2;
    int startX = buttonX + (420 - MeasureText(startText, 30)) / 2;
    int quitX = buttonX + (420 - MeasureText(quitText, 30)) / 2;

    ClearBackground(WHITE);

    DrawText(title, titleX, 110, 60, BLACK);

    DrawRectangleLines(buttonX, 350, 420, 82, BLACK);
    DrawRectangleLines(buttonX, 456, 420, 82, BLACK);

    if (mainMenuSelection == 0)
        DrawRectangleLinesEx((Rectangle){ buttonX, 350, 420, 82 }, 3.0f, BLACK);
    else
        DrawRectangleLinesEx((Rectangle){ buttonX, 456, 420, 82 }, 3.0f, BLACK);

    DrawText(startText, startX, 376, 30, BLACK);
    DrawText(quitText, quitX, 482, 30, BLACK);
}

// Reset the Game Selection screen.
void ResetGameSelection(void)
{
    gameSelection = 0;
    gameSelectionBack = false;
}

// Update Game Selection input.
void UpdateGameSelection(void)
{
    int buttonX = (GetScreenWidth() - 460) / 2;
    Rectangle cannonButton = { buttonX, 285, 460, 72 };
    Rectangle underwaterButton = { buttonX, 377, 460, 72 };
    Rectangle colliderButton = { buttonX, 469, 460, 72 };
    Vector2 mouse = GetMousePosition();
    float wheel = GetMouseWheelMove();
    bool clicked = false;

    if (IsKeyPressed(KEY_UP) || wheel > 0.0f)
        gameSelection = (gameSelection + 2) % 3;

    if (IsKeyPressed(KEY_DOWN) || wheel < 0.0f)
        gameSelection = (gameSelection + 1) % 3;

    if (IsKeyPressed(KEY_ESCAPE))
        gameSelectionBack = true;

    if (CheckCollisionPointRec(mouse, cannonButton))
    {
        gameSelection = 0;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (CheckCollisionPointRec(mouse, underwaterButton))
    {
        gameSelection = 1;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (CheckCollisionPointRec(mouse, colliderButton))
    {
        gameSelection = 2;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (IsKeyPressed(KEY_ENTER) || clicked)
    {
        if (gameSelection == 0)
            startCannonThrow();
        else if (gameSelection == 1)
            startUnderwaterEscape();
        else
            startCollider();
    }
}

// Check if Escape was pressed.
bool IsGameSelectionReturnRequested(void)
{
    return gameSelectionBack;
}

// Draw the Game Selection screen.
void DrawGameSelection(void)
{
    char *title = "SELECT GAME MODE";
    char *cannonText = "Cannon Throw";
    char *underwaterText = "Underwater Escape";
    char *colliderText = "Collider";
    int buttonX = (GetScreenWidth() - 460) / 2;
    int titleX = (GetScreenWidth() - MeasureText(title, 40)) / 2;
    int cannonX = buttonX + (460 - MeasureText(cannonText, 28)) / 2;
    int underwaterX = buttonX + (460 - MeasureText(underwaterText, 28)) / 2;
    int colliderX = buttonX + (460 - MeasureText(colliderText, 28)) / 2;

    ClearBackground(WHITE);

    DrawText(title, titleX, 115, 40, BLACK);

    DrawRectangleLines(buttonX, 285, 460, 72, BLACK);
    DrawRectangleLines(buttonX, 377, 460, 72, BLACK);
    DrawRectangleLines(buttonX, 469, 460, 72, BLACK);

    if (gameSelection == 0)
        DrawRectangleLinesEx((Rectangle){ buttonX, 285, 460, 72 }, 3.0f, BLACK);
    else if (gameSelection == 1)
        DrawRectangleLinesEx((Rectangle){ buttonX, 377, 460, 72 }, 3.0f, BLACK);
    else
        DrawRectangleLinesEx((Rectangle){ buttonX, 469, 460, 72 }, 3.0f, BLACK);

    DrawText(cannonText, cannonX, 307, 28, BLACK);
    DrawText(underwaterText, underwaterX, 399, 28, BLACK);
    DrawText(colliderText, colliderX, 491, 28, BLACK);
}
