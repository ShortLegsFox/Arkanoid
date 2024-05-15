#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int vies;
extern int score_joueur_unite;
extern int score_joueur_dizaine;
extern int score_joueur_centaine;
extern int score_joueur_milliers;
extern int score_joueur_dix_milliers;
extern bool bonus_s;
extern int coord_x_brique_cassee;
extern int coord_y_brique_cassee;

void Recupere_Niveau(const char* nomFichier);
void Ajuster_Score();
void Collision_Balle_Brique();
bool Bonus_S();

#endif // GAME_MANAGER_H