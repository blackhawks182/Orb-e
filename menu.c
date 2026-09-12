#include "menu.h"
#include "raylib.h"

int ShowGameSelectionMenu(void) {
    int centerX = GetScreenWidth() / 2;
    Rectangle enterButton = { centerX - 150.0f, 300.0f, 300.0f, 60.0f };
    Rectangle exitButton = { centerX - 150.0f, 390.0f, 300.0f, 60.0f };

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (IsKeyPressed(KEY_ENTER) ||
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, enterButton))) {
            return 1;
        }

        if (IsKeyPressed(KEY_ESCAPE) ||
            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, exitButton))) {
            return 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        int titleWidth = MeasureText("ORB-E", 60);
        DrawText("ORB-E", (GetScreenWidth() / 2) - (titleWidth / 2), 140, 60, RAYWHITE);

        Color enterColor = CheckCollisionPointRec(mouse, enterButton) ? LIME : GREEN;
        DrawRectangleRec(enterButton, enterColor);
        DrawRectangleLinesEx(enterButton, 2, RAYWHITE);
        DrawText("ENTER", centerX - MeasureText("ENTER", 30) / 2, 315, 30, BLACK);

        Color exitColor = CheckCollisionPointRec(mouse, exitButton) ? LIGHTGRAY : GRAY;
        DrawRectangleRec(exitButton, exitColor);
        DrawRectangleLinesEx(exitButton, 2, RAYWHITE);
        DrawText("EXIT", centerX - MeasureText("EXIT", 30) / 2, 405, 30, BLACK);

        DrawText("Press Enter or click ENTER", centerX - MeasureText("Press Enter or click ENTER", 20) / 2, 520, 20, LIGHTGRAY);

        EndDrawing();
    }

    return 0;
}
