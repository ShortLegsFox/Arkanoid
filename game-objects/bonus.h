#ifndef BONUS_H
#define BONUS_H

#include <stdbool.h>

extern bool bonus_en_cours;

struct Bonus {
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
};

extern struct Bonus stats_bonus;
void Initialise_Bonus(int coord_x, int coord_y);
void Tomber_Bonus();

#endif