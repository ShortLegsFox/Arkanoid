#include "bonus.h"

#include "ball.h"
#include "../game-manager/game_manager.h"
#include "../rendering/rendering.h"
#include "../game-objects/fireshot.h"

struct Bonus stats_bonus;
struct Bonus objetBonus[100];
int indexBonusDansTableau = 0;
bool animationBonus;
int timer = 0;
bool catch_ball = false;

void Initialise_Bonus(int coord_x, int coord_y, char type_bonus, SDL_Rect sourceTexture) {
    struct Bonus bonusAStocker;
    bonusAStocker.pos_x = coord_x;
    bonusAStocker.pos_y = coord_y;
    bonusAStocker.vitesse_x = 2.0;
    bonusAStocker.vitesse_y = 2.4;
    bonusAStocker.type = type_bonus;
    bonusAStocker.indexDansTableau = indexBonusDansTableau;
    bonusAStocker.sourceTexture = sourceTexture;
    bonusAStocker.timerAnimation = 0;
    bonusAStocker.animation = true;
    animationBonus = true;
    objetBonus[indexBonusDansTableau] = bonusAStocker;
    indexBonusDansTableau++;
}

void Met_A_Jour_Position_Bonus(struct Bonus *objetBonus) {
    if(objetBonus->animation) {
        objetBonus->pos_y += objetBonus->vitesse_y;// / delta_t;
        SDL_Rect bonusRect = {objetBonus->pos_x, objetBonus->pos_y, objetBonus->sourceTexture.w, objetBonus->sourceTexture.h};
        SDL_BlitSurface(textures_objets, &objetBonus->sourceTexture, surface_fenetre, &bonusRect);

        if(objetBonus->timerAnimation % 5 == 0) {
            if (objetBonus->sourceTexture.x == 480) {
                objetBonus->sourceTexture.x = 256;
            } else {
                objetBonus->sourceTexture.x += 32;
            }
        }

        if(objetBonus->timerAnimation > 100) {
            objetBonus->timerAnimation = 0;
        }

        objetBonus->timerAnimation++;
    }
}

void Quel_Bonus(char type_bonus) {
    switch (type_bonus) {
        case('S'):
            Bonus_Slow_Down();
            break;
        case('L'):
            Bonus_Laser();
            printf("Laser\n");
            break;
        case('C'):
            Bonus_Catch_Ball();
            printf("Catch\n");
            break;
        default:
            printf("AAAAA\n");
            break;
    }
}

void Bonus_Slow_Down() {
    if(stats_balle.vitesse_y < 0) {
        stats_balle.vitesse_y = -4;
    } else {
        stats_balle.vitesse_y = 4;
    }
}

void Bonus_Catch_Ball() {
    Initialise_Balle();
}

void Bonus_Laser() {
    Initialise_Tableau_Projectiles();
}