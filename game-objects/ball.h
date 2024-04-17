#ifndef BALL_H
#define BALL_H

struct Balle {
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
};

extern struct Balle stats_balle;

void Initialise_Balle();
void Deplace_Balle();

#endif