#ifndef BONUS_H
#define BONUS_H

#include "../rendering/rendering.h"
#include <stdbool.h>

extern bool animationBonus;

struct Bonus {
    char type;
    double pos_x;
    double pos_y;
    double vitesse_x;
    double vitesse_y;
    int indexDansTableau;
    int timerAnimation;
    SDL_Rect sourceTexture;
};

extern struct Bonus stats_bonus;
extern struct Bonus objetBonus[100];
extern int indexBonusDansTableau;
void Initialise_Bonus(int coord_x, int coord_y, char type_bonus, SDL_Rect sourceTexture);
void Met_A_Jour_Position_Bonus(struct Bonus *objetBonus);
void Quel_Bonus(char type_bonus);
void Bonus_Slow_Down();

#endif