#ifndef BALL_H
#define BALL_H
#include <stdbool.h>

struct Balle {
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
};

extern struct Balle stats_balle;

extern bool balle_sur_vaisseau;

void Initialise_Balle();
void Met_A_Jour_Position_Balle();
void Tirer_Balle();

#endif