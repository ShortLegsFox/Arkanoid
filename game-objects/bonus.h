#ifndef BONUS_H
#define BONUS_H

#include <stdbool.h>

extern bool animationBonus;

struct Bonus {
    char type;
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
};

extern struct Bonus stats_bonus;
void Initialise_Bonus(int coord_x, int coord_y, char type_bonus);
void Met_A_Jour_Position_Bonus();
void Quel_Bonus(char type_bonus);
void Bonus_Slow_Down();

#endif