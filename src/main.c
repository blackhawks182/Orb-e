#include "raylib.h"
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define ORB_SIZE 20.0f
#define MOVE_SPEED 350.0f // Speed of direct movement in pixels per second

typedef struct Orb {
    Vector2 position;
    float radius;
    float rotation; // Maintained purely to point the visual indicator where you move
} Orb;


int main(void) {
    // 1. Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Orb-e");
    SetTargetFPS(60);

	Orb player = {0};
    player.position = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
    player.radius = ORB_SIZE;
    player.rotation = 0.0f; // Starts facing up

    
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