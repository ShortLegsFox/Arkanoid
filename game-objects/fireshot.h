#ifndef FIRESHOT_H
#define FIRESHOT_H

#include "../game-manager/game_manager.h"
#include <stdbool.h>

struct Projectile {
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
    SDL_Rect sourceTexture;
    bool actif;
};

extern struct Projectile stats_projectile;
extern struct Projectile objetsProjectiles[10];
void Initialise_Projectile();
void Initialise_Tableau_Projectiles();
void Met_A_Jour_Position_Projectile(struct Projectile *objetProjectile);

#endif
