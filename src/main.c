#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define SHIP_SIZE 20.0f
#define MOVE_SPEED 350.0f // Speed of direct movement in pixels per second

typedef struct Orb {
    Vector2 position;
    float radius;
    float rotation; // Maintained purely to point the visual indicator where you move
} Orb;


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