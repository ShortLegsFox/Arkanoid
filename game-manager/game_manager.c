#include "game_manager.h"
#include "../game-objects/bricks.h"
#include "../game-objects/ball.h"
#include "../rendering/rendering.h"


int vies = 2;
int score_joueur_unite = 0;
int score_joueur_dizaine = 0;
int score_joueur_centaine = 0;
int score_joueur_milliers = 0;
int score_joueur_dix_milliers = 0;

void Recupere_Niveau(const char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier niveau");
        return;
    }

    char ligne[256];
    int largeurMax = 100;
    bool nextLine = false;

    int y = 0;
    int x = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nextLine = false;
        while(!nextLine) {
            if (ligne[x] == '#') {
                briques[y][x].pos_x = x * 30;
                briques[y][x].pos_y = y * 14;
                briques[y][x].estBrique = true; // Brick
            } else {
                briques[y][x].pos_x = x * 30;
                briques[y][x].pos_y = y * 14;
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

void Ajuster_Score() {
    if (score_joueur_unite > 9) {
        score_joueur_unite = 0;
        score_joueur_dizaine ++;
    }
    if (score_joueur_dizaine > 9) {
        score_joueur_dizaine= 0;
        score_joueur_centaine ++;
    }
    if (score_joueur_centaine > 9) {
        score_joueur_centaine = 0;
        score_joueur_milliers ++;
    }
    if (score_joueur_milliers > 9) {
        score_joueur_milliers = 0;
        score_joueur_dix_milliers ++;
    }
}

//Casse les briques lors de la colision
void Collision_Balle_Brique() {
    SDL_Rect balleRect = { stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h };
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                SDL_Rect briqueRect = { briques[i][j].pos_x, briques[i][j].pos_y, source_texture_brique.w, source_texture_brique.h };
                if (SDL_HasIntersection(&balleRect, &briqueRect)) {
                    stats_balle.vitesse_y *= -1;
                    briques[i][j].estBrique = false;    // Marque la brique comme cassée
                    score_joueur_unite++;
                    Ajuster_Score();
                    return;
                }
            }
        }
    }
}