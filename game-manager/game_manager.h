#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int vies;
extern int score_joueur;
extern int max_briques;
extern int count_speed_up;
extern int niveau_actuel;

extern bool bonus_s;
extern bool bonus_l;
extern int coord_x_brique_cassee;
extern int coord_y_brique_cassee;

extern int coord_x_brique_solide_touchee;
extern int coord_y_brique_solide_touchee;
extern SDL_Rect bonusTableau[100];
extern int indexBonus;

void Charge_Niveau(const char* nomFichier);
void Ajuster_Score();
void Verifie_Collision_Balle_Brique();
bool Bonus_S();
bool Niveau_Complet();
void Niveau_Suivant();
void Passe_Niveau();
void Aleatoire_Bonus();
void Casse_La_Brique(int i, int j);
void Incremente_Score(int i, int j);

#endif // GAME_MANAGER_H