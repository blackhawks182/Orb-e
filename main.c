#include "menu.h"
#include "orb_e_game.h"
#include "raylib.h"

int main(void) {
    InitWindow(1200, 800, "Orb-e");
    SetTargetFPS(60);

    int selected = ShowGameSelectionMenu();
    if (selected == 1) {
        RunOrbEGame();
    }

    CloseWindow();
    return 0;
}

