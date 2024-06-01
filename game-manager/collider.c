#include <math.h>
#include "collider.h"
#include "../game-objects/ball.h"
#include "../rendering/rendering.h"
#include "../game-objects/ship.h"
#include "game_manager.h"
#include "../game-objects/bonus.h"
#include "../game-objects/bricks.h"
#include "../utils/utils.h"

#define BALL_SPEED 5 // Adjust as necessary

void Gestion_Collision_Balle_Bord() {
    if (stats_balle.pos_x < 15) {
        stats_balle.pos_x = 15; // Move ball away from the wall
        stats_balle.vitesse_x *= -1;
        premiere_collision_vaisseau = false;
    }
    else if (stats_balle.pos_x > (surface_fenetre->w - source_texture_balle.w - 15)) {
        stats_balle.pos_x = surface_fenetre->w - source_texture_balle.w - 15; // Move ball away from the wall
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

    if (SDL_HasIntersection(&balle, &vaisseau) && !premiere_collision_vaisseau) {
        // Calculate the hit position relative to the center of the paddle
        float collisionX = balle.x + balle.w / 2;
        float vaisseauX = vaisseau.x + vaisseau.w / 2;
        float hitPos = (collisionX - vaisseauX) / (vaisseau.w / 2); // Normalize hit position to range [-1, 1]

        // Adjust ball velocity based on hit position
        float speed = sqrt(stats_balle.vitesse_x * stats_balle.vitesse_x + stats_balle.vitesse_y * stats_balle.vitesse_y);
        float angle = hitPos * (M_PI / 3); // Max angle of deflection = 60 degrees
        stats_balle.vitesse_x = speed * sin(angle);
        stats_balle.vitesse_y = -speed * cos(angle); // Ensure the ball always bounces upwards

        premiere_collision_vaisseau = true;
    } else if (!SDL_HasIntersection(&balle, &vaisseau)) {
        // Reset the flag when the ball is no longer intersecting the paddle
        premiere_collision_vaisseau = false;
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
        char type = stats_bonus.type;
        Quel_Bonus(type);
        stats_bonus.pos_y += 1000;
        score_joueur += 1000;
    }
}

void Gestion_Collision_Bonus_Sortie_Bas() {
    if (stats_bonus.pos_y > (surface_fenetre->h - source_texture_brique_bonus_s.h)) {
        animationBonus = false;
    }
}

void Collision_Balle_Briques(int i, int j) {
    SDL_Rect briqueRect = { briques[i][j].pos_x, briques[i][j].pos_y, source_texture_brique.w, source_texture_brique.h };
    SDL_Rect balleRect = { stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h };

    if (SDL_HasIntersection(&balleRect, &briqueRect)) {
        // Determine the side of the collision
        float ballCenterX = stats_balle.pos_x + source_texture_balle.w / 2.0;
        float ballCenterY = stats_balle.pos_y + source_texture_balle.h / 2.0;
        float brickCenterX = briques[i][j].pos_x + source_texture_brique.w / 2.0;
        float brickCenterY = briques[i][j].pos_y + source_texture_brique.h / 2.0;

        float dx = ballCenterX - brickCenterX;
        float dy = ballCenterY - brickCenterY;

        float absDx = fabs(dx);
        float absDy = fabs(dy);

        if (absDx > absDy) {
            // Horizontal collision
            stats_balle.vitesse_x *= -1;
            // Move the ball out of the brick
            if (dx > 0) {
                stats_balle.pos_x = briques[i][j].pos_x + source_texture_brique.w;
            } else {
                stats_balle.pos_x = briques[i][j].pos_x - source_texture_balle.w;
            }
        } else {
            // Vertical collision
            stats_balle.vitesse_y *= -1;
            // Move the ball out of the brick
            if (dy > 0) {
                stats_balle.pos_y = briques[i][j].pos_y + source_texture_brique.h;
            } else {
                stats_balle.pos_y = briques[i][j].pos_y - source_texture_balle.h;
            }
        }

        // Always process ball movement, regardless of cooldown
        printf("Brick hit at (%d, %d): pv_brique = %d\n", i, j, briques[i][j].pv_brique);

        if (briques[i][j].pv_brique == 1) {
            Aleatoire_Bonus();
            Casse_La_Brique(i, j);
            Incremente_Score(i, j);
        } else {
            briques[i][j].pv_brique -= 1;
            briques[i][j].animation = true;
            briques[i][j].timer_animation = 0;
        }
    }
}

bool Collision_Enemie_Brique(int index) {
    for(int i = 0; i < 100; i++) {
        for(int j = 0; j < 100; j++) {
            SDL_Rect brique = {briques[i][j].pos_x, briques[i][j].pos_y, source_texture_brique.w, source_texture_brique.h};
            SDL_Rect enemie = {enemies[index].pos_x, enemies[index].pos_y, 32, 32};

            if(SDL_HasIntersection(&brique, &enemie) && briques[i][j].estBrique) {
                return true;
            }
        }
    }
    return false;
}

void Collision_Enemie_Bord(int index) {
    if (enemies[index].pos_x < 15) {
        enemies[index].pos_x = 15;
        enemies[index].vitesse_x *= -1;
    }
    else if (enemies[index].pos_x > (surface_fenetre->w - 32 - 15)) {
        enemies[index].pos_x = surface_fenetre->w - 32 - 15;
        enemies[index].vitesse_x *= -1;
    }
}

void Collision_Enemie_Balle(int index) {
    if(enemies[index].estMort == false) {
        SDL_Rect enemie = {enemies[index].pos_x, enemies[index].pos_y, 32, 32};
        SDL_Rect balle = { stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h };

        if(SDL_HasIntersection(&enemie, &balle)) {
            enemies[index].estMort = true;
            printf("%s\n", "enemy killed");
            enemies[index].explose = true;
        }
    }
}

