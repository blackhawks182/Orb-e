#include "save_load.h"
#include <stdio.h>

int LoadHighScore(void) {
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) return 0;

    int highScore = 0;
    fscanf(file, "%d", &highScore);
    fclose(file);
    return highScore;
}

void SaveHighScore(int highScore) {
    FILE *file = fopen("highscore.txt", "w");
    if (file == NULL) return;

    fprintf(file, "%d", highScore);
    fclose(file);
}
