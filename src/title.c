#include "raylib.h"
#include <stddef.h>

#define TITLE_SCREEN_DURATION 2.0f
#define TITLE_FLASH_DURATION 0.18f
#define ORBITRON_FONT_PATH "resources/fonts/Orbitron-Bold.ttf"

typedef struct {
    int width;
    int height;
    int gap;
    int firstY;
} MenuLayout;

static const MenuLayout mainMenuLayout = { 420, 82, 24, 350 };
static const MenuLayout gameSelectionLayout = { 460, 72, 20, 285 };
static const char *const mainMenuOptions[] = { "START GAME", "QUIT GAME" };
static const char *const gameSelectionOptions[] = {
    "Cannon Throw", "Underwater Escape", "Collider"
};

static float titleScreenTimer;
static bool titleScreenFinished;
static int mainMenuSelection;
static bool mainMenuQuitRequested;
static bool mainMenuStartGameRequested;
static int gameSelection;
static bool gameSelectionReturnRequested;
static Font menuFont;
static bool customMenuFontLoaded;

void StartCannonThrow(void);
void StartUnderwaterEscape(void);
void StartCollider(void);

// Maps each game selection index to its placeholder game entry point.
static void (*const gameStarters[])(void) = {
    StartCannonThrow, StartUnderwaterEscape, StartCollider
};

// Loads the Orbitron menu font or falls back to raylib's default font.
void LoadMenuFont(void)
{
    menuFont = GetFontDefault();
    customMenuFontLoaded = false;

    if (FileExists(ORBITRON_FONT_PATH))
    {
        Font orbitronFont = LoadFontEx(ORBITRON_FONT_PATH, 96, NULL, 0);

        if (IsFontValid(orbitronFont))
        {
            menuFont = orbitronFont;
            customMenuFontLoaded = true;
            SetTextureFilter(menuFont.texture, TEXTURE_FILTER_BILINEAR);
        }
    }
}

// Releases the loaded Orbitron font before the window closes.
void UnloadMenuFont(void)
{
    if (customMenuFontLoaded)
    {
        UnloadFont(menuFont);
        customMenuFontLoaded = false;
    }
}

// Returns a font size that keeps text within a banner's horizontal padding.
static int GetFittedMenuFontSize(const char *text, int maximumSize, int availableWidth, int spacing)
{
    int fontSize = maximumSize;

    while (fontSize > 1 &&
           MeasureTextEx(menuFont, text, (float)fontSize, (float)spacing).x > (float)availableWidth)
    {
        fontSize--;
    }

    return fontSize;
}

// Returns the on-screen rectangle for a menu option.
static Rectangle GetMenuBanner(const MenuLayout *layout, int optionIndex)
{
    return (Rectangle){
        (float)((GetScreenWidth() - layout->width) / 2),
        (float)(layout->firstY + optionIndex * (layout->height + layout->gap)),
        (float)layout->width,
        (float)layout->height
    };
}

// Updates keyboard, mouse-wheel, and pointer selection for a menu.
static int UpdateMenuNavigation(int *selection, int optionCount, const MenuLayout *layout)
{
    float mouseWheelMove = GetMouseWheelMove();
    Vector2 mousePosition = GetMousePosition();

    if (IsKeyPressed(KEY_UP) || mouseWheelMove > 0.0f)
    {
        *selection = (*selection + optionCount - 1) % optionCount;
    }

    if (IsKeyPressed(KEY_DOWN) || mouseWheelMove < 0.0f)
    {
        *selection = (*selection + 1) % optionCount;
    }

    for (int optionIndex = 0; optionIndex < optionCount; optionIndex++)
    {
        if (CheckCollisionPointRec(mousePosition, GetMenuBanner(layout, optionIndex)))
        {
            *selection = optionIndex;
            return optionIndex;
        }
    }

    return -1;
}

// Draws a centered title with the existing offset highlight.
static void DrawMenuTitle(const char *title, int fontSize, int spacing, float y)
{
    Vector2 textSize = MeasureTextEx(menuFont, title, (float)fontSize, (float)spacing);
    Vector2 position = { ((float)GetScreenWidth() - textSize.x) * 0.5f, y };

    DrawTextEx(menuFont, title, position, (float)fontSize, (float)spacing, BLACK);
    DrawTextEx(menuFont, title, (Vector2){ position.x - 3.0f, position.y - 3.0f },
               (float)fontSize, (float)spacing, LIGHTGRAY);
}

// Draws one menu banner with its unchanged 3D-style layers.
static void DrawMenuBanner(const char *text, const MenuLayout *layout, int optionIndex,
                           bool selected, int maximumFontSize, int spacing)
{
    Rectangle banner = GetMenuBanner(layout, optionIndex);
    int fontSize = GetFittedMenuFontSize(text, maximumFontSize, layout->width - 32, spacing);
    Vector2 textSize = MeasureTextEx(menuFont, text, (float)fontSize, (float)spacing);
    Vector2 textPosition = {
        banner.x + (banner.width - textSize.x) * 0.5f,
        banner.y + (banner.height - textSize.y) * 0.5f
    };
    Color faceColor = selected ? SKYBLUE : LIGHTGRAY;
    Color borderColor = selected ? BLUE : DARKGRAY;

    DrawRectangle((int)banner.x + 7, (int)banner.y + 7, (int)banner.width, (int)banner.height,
                  DARKGRAY);
    DrawRectangleRec(banner, faceColor);
    DrawRectangleLinesEx(banner, selected ? 4.0f : 3.0f, borderColor);
    DrawTextEx(menuFont, text, (Vector2){ textPosition.x + 3.0f, textPosition.y + 3.0f },
               (float)fontSize, (float)spacing, DARKGRAY);
    DrawTextEx(menuFont, text, textPosition, (float)fontSize, (float)spacing, BLACK);
}

// Draws a menu title and all of its 3D-style option banners.
static void DrawMenu(const char *title, const char *const options[], int optionCount,
                     int titleFontSize, int optionFontSize, int spacing, float titleY,
                     const MenuLayout *layout, int selection)
{
    ClearBackground(WHITE);
    DrawMenuTitle(title, titleFontSize, spacing, titleY);

    for (int optionIndex = 0; optionIndex < optionCount; optionIndex++)
    {
        DrawMenuBanner(options[optionIndex], layout, optionIndex, optionIndex == selection,
                       optionFontSize, spacing);
    }
}

// Resets title screen timing before the state starts.
void ResetTitleScreen(void)
{
    titleScreenTimer = 0.0f;
    titleScreenFinished = false;
}

// Updates the title screen timer and handles transition readiness.
void UpdateTitleScreen(void)
{
    titleScreenTimer += GetFrameTime();

    if (titleScreenTimer >= TITLE_SCREEN_DURATION)
    {
        titleScreenTimer = TITLE_SCREEN_DURATION;
        titleScreenFinished = true;
    }
}

// Returns whether the title screen has completed.
bool IsTitleScreenFinished(void)
{
    return titleScreenFinished;
}

// Draws centered 3D-style title text with a brief starting flash.
void DrawTitleScreen(void)
{
    const char *title = "ORB-E";
    const int fontSize = 96;
    const int spacing = 4;
    Vector2 textSize = MeasureTextEx(menuFont, title, (float)fontSize, (float)spacing);
    Vector2 position = {
        ((float)GetScreenWidth() - textSize.x) * 0.5f,
        ((float)GetScreenHeight() - textSize.y) * 0.5f
    };

    ClearBackground(WHITE);

    if (titleScreenTimer < TITLE_FLASH_DURATION)
    {
        DrawTextEx(menuFont, title, (Vector2){ position.x - 12.0f, position.y - 12.0f },
                   (float)fontSize, (float)spacing, Fade(YELLOW, 0.85f));
        DrawTextEx(menuFont, title, (Vector2){ position.x + 12.0f, position.y + 12.0f },
                   (float)fontSize, (float)spacing, Fade(SKYBLUE, 0.75f));
    }

    for (int offset = 8; offset > 0; offset--)
    {
        DrawTextEx(menuFont, title, (Vector2){ position.x + (float)offset, position.y + (float)offset },
                   (float)fontSize, (float)spacing, DARKGRAY);
    }

    DrawTextEx(menuFont, title, position, (float)fontSize, (float)spacing, BLACK);
    DrawTextEx(menuFont, title, (Vector2){ position.x - 3.0f, position.y - 3.0f },
               (float)fontSize, (float)spacing, LIGHTGRAY);
}

// Resets Main Menu selection and request state before it is shown.
void ResetMainMenu(void)
{
    mainMenuSelection = 0;
    mainMenuQuitRequested = false;
    mainMenuStartGameRequested = false;
}

// Updates Main Menu navigation and confirms the selected action.
void UpdateMainMenu(void)
{
    int hoveredOption = UpdateMenuNavigation(&mainMenuSelection, 2, &mainMenuLayout);

    if (IsKeyPressed(KEY_ENTER) ||
        (hoveredOption >= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
    {
        if (mainMenuSelection == 0)
        {
            mainMenuStartGameRequested = true;
        }
        else
        {
            mainMenuQuitRequested = true;
        }
    }
}

// Returns whether the selected Main Menu action is to close the game.
bool IsMainMenuQuitRequested(void)
{
    return mainMenuQuitRequested;
}

// Returns whether the Main Menu requested the Game Selection screen.
bool IsMainMenuStartGameRequested(void)
{
    return mainMenuStartGameRequested;
}

// Draws the Main Menu title and its option banners.
void DrawMainMenu(void)
{
    DrawMenu("ORB-E", mainMenuOptions, 2, 72, 36, 3, 110.0f, &mainMenuLayout, mainMenuSelection);
}

// Resets Game Selection navigation and return state before it is shown.
void ResetGameSelection(void)
{
    gameSelection = 0;
    gameSelectionReturnRequested = false;
}

// Updates Game Selection navigation and starts its selected placeholder.
void UpdateGameSelection(void)
{
    int hoveredOption = UpdateMenuNavigation(&gameSelection, 3, &gameSelectionLayout);

    if (IsKeyPressed(KEY_ESCAPE))
    {
        gameSelectionReturnRequested = true;
    }

    if (IsKeyPressed(KEY_ENTER) ||
        (hoveredOption >= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
    {
        gameStarters[gameSelection]();
    }
}

// Returns whether Game Selection should return to the Main Menu.
bool IsGameSelectionReturnRequested(void)
{
    return gameSelectionReturnRequested;
}

// Draws the Game Selection title and its option banners.
void DrawGameSelection(void)
{
    DrawMenu("SELECT GAME MODE", gameSelectionOptions, 3, 48, 32, 3, 115.0f,
             &gameSelectionLayout, gameSelection);
}
