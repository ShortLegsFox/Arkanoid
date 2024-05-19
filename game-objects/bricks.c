#include "bricks.h"
#include "../game-manager/game_manager.h"
#include "../rendering/rendering.h"

int timer_brique = 0;
stats_brique briques[100][100];

void Initialise_Brique(int x, int y, int code_couleur) {
    briques[y][x].pos_x = x * source_texture_brique.w + src_bordure_verticale.w;
    briques[y][x].pos_y = y * source_texture_brique.h + topMargin;
    briques[y][x].code_couleur = code_couleur;
    briques[y][x].pv_brique = 1;
    briques[y][x].score = 5;
    briques[y][x].estBrique = true;
    briques[y][x].estSolide = false;
}

void Initialise_Brique_Solide(int x, int y, int code_couleur) {
    briques[y][x].pos_x = x * source_texture_brique.w + src_bordure_verticale.w;
    briques[y][x].pos_y = y * source_texture_brique.h + topMargin;
    briques[y][x].code_couleur = code_couleur;
    briques[y][x].pv_brique = 3;
    briques[y][x].score = 15;
    briques[y][x].timer_animation = 0;
    briques[y][x].animation = false;
    briques[y][x].estBrique = true;
    briques[y][x].estSolide = true;
}

void Initialise_Position_Vide(int x, int y) {
    briques[y][x].estBrique = false;
}

void Commence_Animation(int x, int y) {
    briques[y][x].animation = true;
    briques[y][x].timer_animation = 0;
}

void Animation_Brique_Solide() {
    for(int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if(timer_brique % 5 == 0) {
                briques[i][j].timer_animation += 1;
            }
            if (briques[i][j].animation && briques[i][j].timer_animation < 5 && briques[i][j].estBrique == true) {
                int valeur_index_y_brique = briques[i][j].code_couleur / 6;
                source_texture_brique.x = source_texture_brique.w * briques[i][j].timer_animation;
                source_texture_brique.y = source_texture_brique.h * valeur_index_y_brique;
                Dessine_Texture(source_texture_brique, briques[i][j].pos_x, briques[i][j].pos_y);
            }
        }
    }
    timer_brique++;
}