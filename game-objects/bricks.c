#include "bricks.h"
#include "../game-manager/game_manager.h"
#include "../rendering/rendering.h"

stats_brique briques[100][100];

void Initialise_Brique(int x, int y, int code_couleur) {
    briques[y][x].pos_x = x * source_texture_brique.w + src_bordure_verticale.w;
    briques[y][x].pos_y = y * source_texture_brique.h + topMargin;
    briques[y][x].code_couleur = code_couleur;
    briques[y][x].pv_brique = 1;
    briques[y][x].estBrique = true;
    briques[y][x].estSolide = false;
}

void Initialise_Brique_Solide(int x, int y, int code_couleur) {
    briques[y][x].pos_x = x * source_texture_brique.w + src_bordure_verticale.w;
    briques[y][x].pos_y = y * source_texture_brique.h + topMargin;
    briques[y][x].code_couleur = code_couleur;
    briques[y][x].pv_brique = 3;
    briques[y][x].estBrique = true;
    briques[y][x].estSolide = true;
}

void Initialise_Position_Vide(int x, int y) {
    briques[y][x].estBrique = false;
}