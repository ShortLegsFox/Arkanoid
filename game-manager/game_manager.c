#include <time.h>
#include "game_manager.h"
#include "../game-objects/bricks.h"
#include "../game-objects/ball.h"
#include "../rendering/rendering.h"


int vies = 2;
int score_joueur = 0;
int max_briques = 100;

int coord_x_brique_cassee;
int coord_y_brique_cassee;
bool bonus_s = false;

void Charge_Niveau(const char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier niveau");
        return;
    }

    char ligne[256];
    int largeurMax = surface_fenetre->w;
    bool nextLine = false;

    int y = 0;
    int x = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nextLine = false;
        while(!nextLine) {
            if (ligne[x] == '#' && x * 32 < largeurMax - 32) {
                briques[y][x].pos_x = x * 32 + 15;
                briques[y][x].pos_y = y * 16;
                briques[y][x].estBrique = true; // Brick
            }
            else {
                briques[y][x].pos_x = x * 32;
                briques[y][x].pos_y = y * 16;
                briques[y][x].estBrique = false; // Empty
            }

            if(ligne[x] == 'F') {
                y++;
                nextLine = true;
            }
            x++;
        }
        x = 0;
    }
}

void Incremente_Score() {
    score_joueur++;
}

void Aleatoire_Bonus() {
    srand(time(NULL));
    int r = rand() % 2;

    if(r == 1) {
        bonus_s = true;
    }
    else {
        coord_x_brique_cassee = 0;
        coord_y_brique_cassee = 0;
        bonus_s = false;
    }
}

void Casse_La_Brique(int i, int j) {
    briques[i][j].estBrique = false;    // Marque la brique comme cassée
    coord_x_brique_cassee = i;
    coord_y_brique_cassee = j;
}

void Verifie_si_brique(int i, int j) {
    if (briques[i][j].estBrique) {
        SDL_Rect briqueRect = { briques[i][j].pos_x, briques[i][j].pos_y, source_texture_brique.w, source_texture_brique.h };
        SDL_Rect balleRect = { stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h };
        if (SDL_HasIntersection(&balleRect, &briqueRect)) {
            stats_balle.vitesse_y *= -1;
            Aleatoire_Bonus();
            Casse_La_Brique(i,j);
            Incremente_Score();
            return;
        }
    }
}

void Collision_Balle_Brique() {
    bonus_s = false;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Verifie_si_brique(i,j);
        }
    }
}