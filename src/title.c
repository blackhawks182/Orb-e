#include "raylib.h"

#define TITLE_SCREEN_DURATION 3.0f
#define TITLE_FLASH_DURATION 0.18f

static float titleScreenTimer = 0.0f;
static bool titleScreenFinished = false;

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
