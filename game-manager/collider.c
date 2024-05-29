#include "collider.h"
#include "../game-objects/ball.h"
#include "../rendering/rendering.h"
#include "../game-objects/ship.h"
#include "../game-objects/ball.h"
#include "game_manager.h"
#include "../game-objects/bonus.h"

void Gestion_Collision_Balle_Bord() {
    if ((stats_balle.pos_x < 15) || (stats_balle.pos_x > (surface_fenetre->w - source_texture_balle.w - 15))) {
        stats_balle.vitesse_x *= -1;
        premiere_collision_vaisseau = false;
    }
}

void Gestion_Collision_Balle_Haut() {
    if ((stats_balle.pos_y < 15 + topMargin)) {
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = false;
    }
}

void Gestion_Collision_Balle_Vaisseau() {
    SDL_Rect vaisseau = {x_pos_vaisseau, surface_fenetre->h - 40, source_texture_vaisseau.w, source_texture_vaisseau.h};
    SDL_Rect balle = {stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h};

    if ((SDL_HasIntersection(&balle, &vaisseau)) && premiere_collision_vaisseau == false){
        int collisionX = balle.x + balle.w / 2; // Position x de la collision
        int vaisseauX = vaisseau.x + vaisseau.w / 2; // Position x du centre du vaisseau

        if (collisionX < vaisseauX) {
            stats_balle.vitesse_x = -abs(stats_balle.vitesse_x);
        } else {
            stats_balle.vitesse_x = abs(stats_balle.vitesse_x);
        }
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = true;
    }
}

void Gestion_Collision_Balle_Sortie_Bas() {
    if (stats_balle.pos_y > (surface_fenetre->h - source_texture_balle.h)) {
        vies--;
        Initialise_Balle();
    }
}

void Gestion_Collision_Bonus_Vaisseau() {
    SDL_Rect vaisseau = {x_pos_vaisseau, surface_fenetre->h - 40, source_texture_vaisseau.w, source_texture_vaisseau.h};
    SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w, source_texture_brique_bonus_s.h};

    if(SDL_HasIntersection(&vaisseau, &bonus)) {
        animationBonus = false;
        // C'est moche, mais ça marche
        stats_bonus.pos_y += 1000;
        score_joueur += 1000;
    }
}

void Gestion_Collision_Bonus_Sortie_Bas() {
    if (stats_bonus.pos_y > (surface_fenetre->h - source_texture_brique_bonus_s.h)) {
        animationBonus = false;
    }
}