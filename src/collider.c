#include "raylib.h"

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

void startCollider(void)
{
    int textWidth = MeasureText("Coming Soon!", 40);
    while(!WindowShouldClose() && !IsKeyPressed(KEY_ZERO)){
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Coming Soon!", SCREEN_WIDTH/2 - textWidth/2, SCREEN_HEIGHT/2, 40, RAYWHITE);
        EndDrawing();
    }
}
