#include <time.h>
#include "game_manager.h"

#include "collider.h"
#include "../game-objects/bricks.h"
#include "../game-objects/ball.h"
#include "../game-objects/bonus.h"
#include "../rendering/rendering.h"
#include "../game-objects/ship.h"
#include "../game-objects/fireshot.h"


int vies = 2;
int score_joueur = 0;
int max_briques = 100;
int count_speed_up = 0;
int niveau_actuel = 1;

int coord_x_brique_cassee;
int coord_y_brique_cassee;

int coord_x_brique_solide_touchee;
int coord_y_brique_solide_touchee;

bool bonus_s = false;
bool bonus_l = false;
bool bonus_c = false;

SDL_Rect bonusTableau[100];
int indexBonus = 0;

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
                Initialise_Brique_Indestructible(x, y, 18);
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
    int r = rand() % 15;

    if(r == 0) {
        bonus_c = true;
    }
    else if(r == 5) {
        bonus_l = true;
    }
    else if (r == 10) {
        bonus_s = true;
    }
    else {
        coord_x_brique_cassee = 0;
        coord_y_brique_cassee = 0;
        bonus_s = false;
        bonus_l = false;
        bonus_c = false;
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

void Verifie_Collision_Balle_Brique() {
    bonus_s = false;
    bonus_l = false;
    bonus_c = false;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                Collision_Balle_Briques(i,j);
            }
        }
    }
}

void Verifie_Collision_Projectile_Brique(struct Projectile * projectile) {
    bonus_s = false;
    bonus_l = false;
    bonus_c = false;
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                Collision_Projectile_Briques(projectile,i,j);
            }
        }
    }
}

bool Niveau_Complet() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique && briques[i][j].estIndestructible == false) {
                return false;
            }
        }
    }
    return true;
}

void Niveau_Suivant() {
    if (Niveau_Complet()) {
        Vide_Tableau_Briques();
        if (niveau_actuel < 6) {    //Verif à pas faire un magic number
            niveau_actuel++;
            char nomFichier[64];
            sprintf(nomFichier, "../niveaux/niveau%d.txt", niveau_actuel);
            Sprite_par_niveau();
            Charge_Niveau(nomFichier);
            Initialise_Balle();
            Initialise_Vaisseau();
            printf("Niveau %d chargé. Bonne chance!\n", niveau_actuel);
        } else {
            printf("Félicitations! Vous avez terminé tous les niveaux!\n");
            Afficher_Victoire();
        }
    }
}

void Niveau_Precedent() {
        Vide_Tableau_Briques();
        if (niveau_actuel > 0) {    //Verif à pas faire un magic number
            niveau_actuel--;
            char nomFichier[64];
            sprintf(nomFichier, "../niveaux/niveau%d.txt", niveau_actuel);
            Sprite_par_niveau();
            Charge_Niveau(nomFichier);
            Initialise_Balle();
            Initialise_Vaisseau();
            printf("Niveau %d chargé. Bonne chance!\n", niveau_actuel);
        } else {
            printf("Vous etes trop revenu en arrière !!!\n");
            Afficher_Game_Over();
        }
}

void Passe_Niveau(){
    Vide_Tableau_Briques();
    Niveau_Complet();
    printf("Triche : Passe niveau. ");
}

void Perd_Niveau(){
    Vide_Tableau_Briques();
    Niveau_Precedent();
    printf("Triche : Niveau precedent");
}