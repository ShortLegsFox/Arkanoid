#include "fireshot.h"
#include "ship.h"
#include "../rendering/rendering.h"


struct Projectile stats_projectile;
struct Projectile objetsProjectiles[10];
int indexProjectile = 10;

void Initialise_Tableau_Projectiles() {
    for(int i = 0; i < 10; i++) {
        objetsProjectiles[i].actif = false;
    }
    indexProjectile = 0;
    printf("Gun loaded");
}

void Initialise_Projectile() {
    if(indexProjectile < 10) {
        objetsProjectiles[indexProjectile].pos_x = x_pos_vaisseau;
        objetsProjectiles[indexProjectile].pos_x_bis = x_pos_vaisseau + source_texture_vaisseau.w - 10;
        objetsProjectiles[indexProjectile].pos_y = y_pos_vaisseau;
        objetsProjectiles[indexProjectile].vitesse_x = 5.5;
        objetsProjectiles[indexProjectile].vitesse_y = -10;
        objetsProjectiles[indexProjectile].sourceTexture = source_texture_projectile;
        objetsProjectiles[indexProjectile].actif = true;
        indexProjectile++;
        printf("%d\n", indexProjectile);
    } else {
        printf("Out of ammo!\n");
    }
}

void Met_A_Jour_Position_Projectile(struct Projectile *objetProjectile) {
    if (objetProjectile->actif)
        objetProjectile->pos_y += objetProjectile->vitesse_y; // / delta_t;
}
