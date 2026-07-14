#include "raylib.h"

#define TITLE_SCREEN_DURATION 3.0f
#define TITLE_FLASH_DURATION 0.18f

static float titleScreenTimer = 0.0f;
static bool titleScreenFinished = false;
static int mainMenuSelection = 0;
static bool mainMenuQuitRequested = false;

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
    Font font = GetFontDefault();
    Vector2 textSize = MeasureTextEx(font, title, (float)fontSize, (float)spacing);
    Vector2 position = {
        ((float)GetScreenWidth() - textSize.x) * 0.5f,
        ((float)GetScreenHeight() - textSize.y) * 0.5f
    };

    ClearBackground(WHITE);

    if (titleScreenTimer < TITLE_FLASH_DURATION)
    {
        DrawTextEx(font, title, (Vector2){ position.x - 12.0f, position.y - 12.0f },
                   (float)fontSize, (float)spacing, Fade(YELLOW, 0.85f));
        DrawTextEx(font, title, (Vector2){ position.x + 12.0f, position.y + 12.0f },
                   (float)fontSize, (float)spacing, Fade(SKYBLUE, 0.75f));
    }

    for (int offset = 8; offset > 0; offset--)
    {
        DrawTextEx(font, title, (Vector2){ position.x + (float)offset, position.y + (float)offset },
                   (float)fontSize, (float)spacing, DARKGRAY);
    }

    DrawTextEx(font, title, position, (float)fontSize, (float)spacing, BLACK);
    DrawTextEx(font, title, (Vector2){ position.x - 3.0f, position.y - 3.0f },
               (float)fontSize, (float)spacing, LIGHTGRAY);
}

// Resets Main Menu selection and quit state before it is shown.
void ResetMainMenu(void)
{
    mainMenuSelection = 0;
    mainMenuQuitRequested = false;
}

// Updates Main Menu keyboard selection and confirmation input.
void UpdateMainMenu(void)
{
    if (IsKeyPressed(KEY_UP))
    {
        mainMenuSelection = 0;
    }

    if (IsKeyPressed(KEY_DOWN))
    {
        mainMenuSelection = 1;
    }

    if (IsKeyPressed(KEY_ENTER))
    {
        if (mainMenuSelection == 0)
        {
            // TODO: Transition to gameplay when that feature is implemented.
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

// Draws the main menu title and its 3D-style option banners.
void DrawMainMenu(void)
{
    const char *title = "ORB-E";
    const char *options[] = { "START GAME", "QUIT GAME" };
    const int titleFontSize = 72;
    const int optionFontSize = 36;
    const int spacing = 3;
    const int bannerWidth = 420;
    const int bannerHeight = 82;
    const int bannerGap = 24;
    const int firstBannerY = 350;
    Font font = GetFontDefault();
    Vector2 titleSize = MeasureTextEx(font, title, (float)titleFontSize, (float)spacing);
    int bannerX = (GetScreenWidth() - bannerWidth) / 2;

    ClearBackground(WHITE);
    DrawTextEx(font, title,
               (Vector2){ ((float)GetScreenWidth() - titleSize.x) * 0.5f, 110.0f },
               (float)titleFontSize, (float)spacing, BLACK);
    DrawTextEx(font, title,
               (Vector2){ ((float)GetScreenWidth() - titleSize.x) * 0.5f - 3.0f, 107.0f },
               (float)titleFontSize, (float)spacing, LIGHTGRAY);

    for (int optionIndex = 0; optionIndex < 2; optionIndex++)
    {
        int bannerY = firstBannerY + optionIndex * (bannerHeight + bannerGap);
        bool selected = optionIndex == mainMenuSelection;
        Color faceColor = selected ? SKYBLUE : LIGHTGRAY;
        Color borderColor = selected ? BLUE : DARKGRAY;
        Vector2 optionSize = MeasureTextEx(font, options[optionIndex], (float)optionFontSize,
                                           (float)spacing);
        Vector2 optionPosition = {
            (float)bannerX + ((float)bannerWidth - optionSize.x) * 0.5f,
            (float)bannerY + ((float)bannerHeight - optionSize.y) * 0.5f
        };

        DrawRectangle(bannerX + 7, bannerY + 7, bannerWidth, bannerHeight, DARKGRAY);
        DrawRectangle(bannerX, bannerY, bannerWidth, bannerHeight, faceColor);
        DrawRectangleLinesEx((Rectangle){ (float)bannerX, (float)bannerY,
                                           (float)bannerWidth, (float)bannerHeight },
                             selected ? 4.0f : 3.0f, borderColor);
        DrawTextEx(font, options[optionIndex],
                   (Vector2){ optionPosition.x + 3.0f, optionPosition.y + 3.0f },
                   (float)optionFontSize, (float)spacing, DARKGRAY);
        DrawTextEx(font, options[optionIndex], optionPosition,
                   (float)optionFontSize, (float)spacing, BLACK);
    }
}
