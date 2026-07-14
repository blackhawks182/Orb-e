#include "raylib.h"

#define SCREEN_WIDTH 1200

#define SCREEN_HEIGHT 800

//projectile throw game loop

int projectileThrow(){
    Rectangle ground = {0,SCREEN_HEIGHT/2, SCREEN_WIDTH, SCREEN_HEIGHT/2};
    int end_projectile_game = 0;
    while (!WindowShouldClose && !end_projectile_game){

        float deltaTime = GetFrameTime();
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangleRec(ground, GRAY);



    }
    
}