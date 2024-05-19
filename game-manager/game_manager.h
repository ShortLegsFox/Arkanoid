#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int vies;
extern int score_joueur;
extern int max_briques;

extern bool bonus_s;
extern int coord_x_brique_cassee;
extern int coord_y_brique_cassee;

extern int coord_x_brique_solide_touchee;
extern int coord_y_brique_solide_touchee;

void Charge_Niveau(const char* nomFichier);
void Ajuster_Score();
void Collision_Balle_Brique();
bool Bonus_S();

#endif // GAME_MANAGER_H