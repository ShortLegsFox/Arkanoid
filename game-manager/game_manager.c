#include <time.h>
#include "game_manager.h"
#include "../game-objects/bricks.h"
#include "../game-objects/ball.h"
#include "../game-objects/bonus.h"
#include "../rendering/rendering.h"
#include "../game-objects/ship.h"


int vies = 2;
int score_joueur = 0;
int max_briques = 100;
int count_speed_up = 0;

int coord_x_brique_cassee;
int coord_y_brique_cassee;

int coord_x_brique_solide_touchee;
int coord_y_brique_solide_touchee;

bool bonus_s = false;

bool Briques_Depassent_Limite_X(int index, int largeur_brique, int largeur_max){
    return index * largeur_brique < largeur_max - largeur_brique;
}

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
            if(ligne[x] == 'F') {
                y++;
                nextLine = true;
            }
            else if (ligne[x] == '#' && Briques_Depassent_Limite_X(x, source_texture_brique.w, largeurMax)) {
                Initialise_Position_Vide(x, y);
            }
            else if (ligne[x] == 'S' && Briques_Depassent_Limite_X(x, source_texture_brique.w, largeurMax)) {
                Initialise_Brique_Solide(x, y, 12);
            }
            else if (ligne[x] == 'G' && Briques_Depassent_Limite_X(x, source_texture_brique.w, largeurMax)) {
                Initialise_Brique_Solide(x, y, 18);
            }
            else if(Briques_Depassent_Limite_X(x, source_texture_brique.w, largeurMax)) {
                Initialise_Brique(x, y, ligne[x] - '0');
            }
            x++;
        }
        x = 0;
    }
}

void Incremente_Score(int x, int y) {
    score_joueur += briques[x][y].score;
}

void Aleatoire_Bonus() {
    srand(time(NULL));
    int r = rand() % 20;

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
    count_speed_up++;
    if(count_speed_up == 10) {
        count_speed_up = 0;
        stats_balle.vitesse_y *= 1.1;
    }
}

#define COOLDOWN_PERIOD 1 // Adjust this value as needed

void Verifie_si_brique(int i, int j) {
    if (briques[i][j].estBrique) {
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

            if (briques[i][j].pv_brique <= 1) {
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
}

void Collision_Balle_Brique() {
    bonus_s = false;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Verifie_si_brique(i,j);
        }
    }
}

bool Niveau_Complet() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                return false;
            }
        }
    }
    return true;
}

void Niveau_Suivant() {
    static int niveau_actuel = 1;
    if (Niveau_Complet()) {
        if (niveau_actuel < 5) { // Suppose qu'il y a 5 niveaux
            niveau_actuel++;
            char nomFichier[64];
            sprintf(nomFichier, "../niveaux/niveau%d.txt", niveau_actuel);
            Charge_Niveau(nomFichier);
            Initialise_Balle();
            Initialise_Vaisseau();
            printf("Niveau %d chargé. Bonne chance!\n", niveau_actuel);
        } else {
            printf("Félicitations! Vous avez terminé tous les niveaux!\n");
            // Ajouter du code ici pour gérer la fin du jeu.
        }
    }
}
