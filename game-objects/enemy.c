#include "enemy.h"
#define NUM_ENEMIES 4
#define ENEMY_HORIZONTAL_SPEED 2.0
#define ENEMY_FALL_SPEED 1.0
#define ENEMY_WIDTH 30
#define ENEMY_HEIGHT 30

void SpawnEnemies(Enemy enemies[], int screen_width, double first_line_y) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].pos_x = (screen_width / NUM_ENEMIES) * i + (screen_width / NUM_ENEMIES) / 2 - ENEMY_WIDTH / 2;
        enemies[i].pos_y = 0;
        enemies[i].vitesse_x = 0;
        enemies[i].vitesse_y = ENEMY_FALL_SPEED;
    }
}