#include "raylib.h"
#include <math.h>

static float titleTimer;
static bool titleFinished;

static int mainMenuSelection;
static bool mainMenuQuit;
static bool mainMenuStart;

static int gameSelection;
static bool gameSelectionBack;
static bool gameSelectionQuit;

static Music menuMusic;
static bool menuMusicLoaded = false;

static Texture2D menuBackground;
static bool menuBackgroundLoaded = false;

static const Color menuButtonColor = { 4, 20, 52, 240 };
static const Color menuSelectedButtonColor = { 5, 57, 87, 245 };
static const Color menuBorderColor = { 116, 224, 240, 255 };
static const Color menuSelectedBorderColor = { 235, 255, 255, 255 };
static const Color menuTextColor = { 235, 255, 255, 255 };
static const Color menuTextShadowColor = { 1, 10, 31, 255 };

typedef struct MenuBubble
{
    Vector2 position;
    float radius;
    float speed;
} MenuBubble;

static MenuBubble menuBubbles[30] =
{
    { { 55.0f, 720.0f }, 5.0f, 16.0f },
    { { 115.0f, 590.0f }, 7.0f, 22.0f },
    { { 175.0f, 760.0f }, 3.0f, 13.0f },
    { { 235.0f, 500.0f }, 8.0f, 19.0f },
    { { 300.0f, 680.0f }, 6.0f, 15.0f },
    { { 335.0f, 390.0f }, 5.0f, 24.0f },
    { { 80.0f, 430.0f }, 4.0f, 11.0f },
    { { 145.0f, 290.0f }, 10.0f, 18.0f },
    { { 205.0f, 170.0f }, 6.0f, 14.0f },
    { { 270.0f, 810.0f }, 7.0f, 21.0f },
    { { 890.0f, 680.0f }, 6.0f, 17.0f },
    { { 945.0f, 540.0f }, 3.0f, 13.0f },
    { { 1005.0f, 760.0f }, 8.0f, 23.0f },
    { { 1065.0f, 430.0f }, 5.0f, 15.0f },
    { { 1130.0f, 610.0f }, 7.0f, 20.0f },
    { { 1170.0f, 780.0f }, 4.0f, 12.0f },
    { { 860.0f, 300.0f }, 10.0f, 19.0f },
    { { 925.0f, 180.0f }, 5.0f, 16.0f },
    { { 990.0f, 350.0f }, 6.0f, 22.0f },
    { { 1080.0f, 90.0f }, 3.0f, 14.0f },
    { { 1145.0f, 240.0f }, 8.0f, 18.0f },
    { { 75.0f, 90.0f }, 6.0f, 20.0f },
    { { 130.0f, 820.0f }, 4.0f, 15.0f },
    { { 195.0f, 440.0f }, 7.0f, 12.0f },
    { { 260.0f, 40.0f }, 5.0f, 17.0f },
    { { 880.0f, 820.0f }, 7.0f, 14.0f },
    { { 955.0f, 70.0f }, 3.0f, 11.0f },
    { { 1025.0f, 820.0f }, 10.0f, 21.0f },
    { { 1100.0f, 500.0f }, 6.0f, 16.0f },
    { { 1160.0f, 120.0f }, 5.0f, 23.0f }
};

static const float menuBubbleSway[30] =
{
    2.0f, -1.5f, 1.0f, -2.0f, 1.5f, -1.0f, 2.0f, -1.5f, 1.0f, -2.0f,
    1.5f, -1.0f, 2.0f, -1.5f, 1.0f, -2.0f, 1.5f, -1.0f, 2.0f, -1.5f,
    1.0f, -2.0f, 1.5f, -1.0f, 2.0f, -1.5f, 1.0f, -2.0f, 1.5f, -1.0f
};

static const float menuBubblePhase[30] =
{
    0.0f, 0.7f, 1.4f, 2.1f, 2.8f, 3.5f, 4.2f, 4.9f, 5.6f, 6.3f,
    0.4f, 1.1f, 1.8f, 2.5f, 3.2f, 3.9f, 4.6f, 5.3f, 6.0f, 0.3f,
    1.0f, 1.7f, 2.4f, 3.1f, 3.8f, 4.5f, 5.2f, 5.9f, 0.6f, 1.3f
};

static const unsigned char menuBubbleAlpha[30] =
{
    72, 112, 88, 128, 96, 64, 118, 82, 136, 90,
    104, 70, 126, 94, 78, 116, 86, 132, 68, 108,
    98, 74, 120, 84, 140, 76, 110, 92, 66, 124
};

void startCannonThrow(void);
void startUnderwaterEscape(void);
void startCollider(void);

void LoadMenuBackground(void)
{
    menuBackground = LoadTexture("assets/menu_background.png");
    menuBackgroundLoaded = menuBackground.id != 0;
}

void UnloadMenuBackground(void)
{
    if (menuBackgroundLoaded)
    {
        UnloadTexture(menuBackground);
        menuBackgroundLoaded = false;
    }
}

static void DrawMenuBackground(void)
{
    if (!menuBackgroundLoaded)
    {
        ClearBackground(WHITE);
        return;
    }

    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();
    float textureWidth = (float)menuBackground.width;
    float textureHeight = (float)menuBackground.height;
    float screenAspect = screenWidth / screenHeight;
    float textureAspect = textureWidth / textureHeight;
    Rectangle source = { 0.0f, 0.0f, textureWidth, textureHeight };

    if (textureAspect > screenAspect)
    {
        source.width = textureHeight * screenAspect;
        source.x = (textureWidth - source.width) / 2.0f;
    }
    else
    {
        source.height = textureWidth / screenAspect;
        source.y = (textureHeight - source.height) / 2.0f;
    }

    DrawTexturePro(
        menuBackground,
        source,
        (Rectangle){ 0.0f, 0.0f, screenWidth, screenHeight },
        (Vector2){ 0.0f, 0.0f },
        0.0f,
        WHITE
    );
}

static void UpdateMenuBubbles(void)
{
    float deltaTime = GetFrameTime();
    float screenHeight = (float)GetScreenHeight();
    float time = (float)GetTime();

    for (int i = 0; i < 30; i++)
    {
        menuBubbles[i].position.x +=
            sinf(time * 0.35f + menuBubblePhase[i]) *
            menuBubbleSway[i] * deltaTime;
        menuBubbles[i].position.y -= menuBubbles[i].speed * deltaTime;

        if (menuBubbles[i].position.y + menuBubbles[i].radius < 0.0f)
        {
            menuBubbles[i].position.y = screenHeight + menuBubbles[i].radius;
        }
    }
}

static void DrawMenuBubbles(void)
{
    for (int i = 0; i < 30; i++)
    {
        Color bubbleColor = { 174, 238, 247, menuBubbleAlpha[i] };
        Color highlightColor =
        {
            225,
            255,
            255,
            (unsigned char)(menuBubbleAlpha[i] * 0.65f)
        };

        DrawCircleLines(
            (int)menuBubbles[i].position.x,
            (int)menuBubbles[i].position.y,
            menuBubbles[i].radius,
            bubbleColor
        );
        DrawCircleV(
            (Vector2){
                menuBubbles[i].position.x - menuBubbles[i].radius * 0.35f,
                menuBubbles[i].position.y - menuBubbles[i].radius * 0.35f
            },
            menuBubbles[i].radius * 0.18f,
            highlightColor
        );
    }
}

static void DrawMenuButton(Rectangle bounds, bool selected)
{
    DrawRectangleRec(
        bounds,
        selected ? menuSelectedButtonColor : menuButtonColor
    );
    DrawRectangleLinesEx(
        bounds,
        selected ? 3.0f : 2.0f,
        selected ? menuSelectedBorderColor : menuBorderColor
    );
}

static void DrawMenuText(const char *text, int x, int y, int fontSize)
{
    Font font = GetFontDefault();
    Vector2 position = { (float)x, (float)y };

    DrawTextEx(
        font,
        text,
        (Vector2){ position.x + 2.0f, position.y + 2.0f },
        (float)fontSize,
        1.0f,
        menuTextShadowColor
    );
    DrawTextEx(font, text, position, (float)fontSize, 1.0f, menuTextColor);
}

static int MeasureMenuText(const char *text, int fontSize)
{
    Font font = GetFontDefault();
    Vector2 size = MeasureTextEx(font, text, (float)fontSize, 1.0f);
    return (int)size.x;
}

// No custom font is used in the simple menu.
void LoadMenuFont(void)
{
}

// No custom font needs to be unloaded.
void UnloadMenuFont(void)
{
}

// Load menu music.
void LoadMenuMusic(void)
{
    menuMusic = LoadMusicStream("assets/menu_music.mp3");

    if (IsAudioDeviceReady())
    {
        menuMusicLoaded = true;
        SetMusicVolume(menuMusic, 0.5f);
    }
}

// Start menu music from the beginning.
void StartMenuMusic(void)
{
    if (menuMusicLoaded)
    {
        StopMusicStream(menuMusic);
        SeekMusicStream(menuMusic, 0.0f);
        PlayMusicStream(menuMusic);
    }
}

// Update menu music every frame.
void UpdateMenuMusic(void)
{
    if (menuMusicLoaded)
    {
        UpdateMusicStream(menuMusic);
    }
}

// Stop menu music.
void StopMenuMusic(void)
{
    if (menuMusicLoaded)
    {
        StopMusicStream(menuMusic);
    }
}

// Unload menu music.
void UnloadMenuMusic(void)
{
    if (menuMusicLoaded)
    {
        UnloadMusicStream(menuMusic);
        menuMusicLoaded = false;
    }
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
    UpdateMenuBubbles();

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
    int titleX = (GetScreenWidth() - MeasureMenuText(title, 60)) / 2;
    int startX = buttonX + (420 - MeasureMenuText(startText, 30)) / 2;
    int quitX = buttonX + (420 - MeasureMenuText(quitText, 30)) / 2;

    DrawMenuBackground();
    DrawMenuBubbles();

    DrawMenuText(title, titleX, 110, 60);

    DrawMenuButton((Rectangle){ buttonX, 350, 420, 82 }, mainMenuSelection == 0);
    DrawMenuButton((Rectangle){ buttonX, 456, 420, 82 }, mainMenuSelection == 1);

    DrawMenuText(startText, startX, 376, 30);
    DrawMenuText(quitText, quitX, 482, 30);
}

// Reset the Game Selection screen.
void ResetGameSelection(void)
{
    gameSelection = 0;
    gameSelectionBack = false;
    gameSelectionQuit = false;
}

// Update Game Selection input.
void UpdateGameSelection(void)
{
    UpdateMenuBubbles();

    int buttonX = (GetScreenWidth() - 460) / 2;

    Rectangle cannonButton = { buttonX, 285, 460, 72 };
    Rectangle underwaterButton = { buttonX, 377, 460, 72 };
    Rectangle colliderButton = { buttonX, 469, 460, 72 };
    Rectangle quitButton = { buttonX, 561, 460, 72 };

    Vector2 mouse = GetMousePosition();
    float wheel = GetMouseWheelMove();
    bool clicked = false;

    if (IsKeyPressed(KEY_UP) || wheel > 0.0f)
        gameSelection = (gameSelection + 3) % 4;

    if (IsKeyPressed(KEY_DOWN) || wheel < 0.0f)
        gameSelection = (gameSelection + 1) % 4;

    // ESC returns to the Main Menu.
    if (IsKeyPressed(KEY_ESCAPE))
    {
        gameSelectionBack = true;
        StartMenuMusic();
    }

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

    if (CheckCollisionPointRec(mouse, quitButton))
    {
        gameSelection = 3;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            clicked = true;
    }

    if (IsKeyPressed(KEY_ENTER) || clicked)
    {
        if (gameSelection == 3)
        {
            gameSelectionQuit = true;
            return;
        }

        // Stop menu music while the actual game is running.
        StopMenuMusic();

        if (gameSelection == 0)
        {
            startCannonThrow();
        }
        else if (gameSelection == 1)
        {
            startUnderwaterEscape();
        }
        else
        {
            startCollider();
        }

        // Game has ended.
        // Stay on the Game Selection screen and restart the music.
        StartMenuMusic();
    }
}

// Check if Escape was pressed.
bool IsGameSelectionReturnRequested(void)
{
    return gameSelectionBack;
}

// Check if Quit Game was selected on the Game Selection screen.
bool IsGameSelectionQuitRequested(void)
{
    return gameSelectionQuit;
}

// Draw the Game Selection screen.
void DrawGameSelection(void)
{
    char *title = "SELECT GAME MODE";
    char *cannonText = "Cannon Throw";
    char *underwaterText = "Underwater Escape";
    char *colliderText = "Collider";
    char *quitText = "QUIT GAME";

    int buttonX = (GetScreenWidth() - 460) / 2;
    int titleX = (GetScreenWidth() - MeasureMenuText(title, 40)) / 2;
    int cannonX = buttonX + (460 - MeasureMenuText(cannonText, 28)) / 2;
    int underwaterX = buttonX + (460 - MeasureMenuText(underwaterText, 28)) / 2;
    int colliderX = buttonX + (460 - MeasureMenuText(colliderText, 28)) / 2;
    int quitX = buttonX + (460 - MeasureMenuText(quitText, 28)) / 2;

    DrawMenuBackground();
    DrawMenuBubbles();

    DrawMenuText(title, titleX, 115, 40);

    DrawMenuButton((Rectangle){ buttonX, 285, 460, 72 }, gameSelection == 0);
    DrawMenuButton((Rectangle){ buttonX, 377, 460, 72 }, gameSelection == 1);
    DrawMenuButton((Rectangle){ buttonX, 469, 460, 72 }, gameSelection == 2);
    DrawMenuButton((Rectangle){ buttonX, 561, 460, 72 }, gameSelection == 3);

    DrawMenuText(cannonText, cannonX, 307, 28);
    DrawMenuText(underwaterText, underwaterX, 399, 28);
    DrawMenuText(colliderText, colliderX, 491, 28);
    DrawMenuText(quitText, quitX, 583, 28);
}