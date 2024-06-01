#ifndef ENEMY_H
#define ENEMY_H
#include <stdbool.h>

typedef struct {
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
    int numero_porte;
    char type;
    int timer_animation;
    bool estMort;
} Enemy;

extern Enemy enemies[];

void Initialise_Enemie(int index, int numero_porte, char type);
void Animation_Enemie_Pyramide_Vert_Verre(int i);
void Animation_Enemie_Chibre_Bleu(int i);
void Animation_Enemie_Chromosome(int i);
void Chute_Enemies(int index);
void Deplacement_Enemies(int index);
void Met_A_Jour_Enemies();

#endif
