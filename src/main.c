#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800

int main(void) {
    // 1. Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Asteroids Milestone 2 - Direct 2D Movement");
    SetTargetFPS(60);

    
    // 2. Main Game Loop
    while (!WindowShouldClose()) {
        
        // ---------------------------------------------------------------------
        // RENDER
        // ---------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        
        EndDrawing();
    }

    // 3. De-Initialization
    CloseWindow();
    return 0;
}