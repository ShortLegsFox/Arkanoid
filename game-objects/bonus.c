#include "bonus.h"
#include "../rendering/rendering.h"

struct Bonus stats_bonus;
bool animation;
int timer = 0;

void Initialise_Bonus(int coord_x, int coord_y) {
    stats_bonus.pos_x = coord_x;
    stats_bonus.pos_y = coord_y;
    stats_bonus.vitesse_x = 2.0;
    stats_bonus.vitesse_y = 2.4;
    animation = true;
}

void Met_A_Jour_Position_Bonus() {
    if(animation) {
        stats_bonus.pos_y += stats_bonus.vitesse_y;// / delta_t;
        SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w,
                          source_texture_brique_bonus_s.h};
        SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);

        if(timer % 5 == 0) {
            if (source_texture_brique_bonus_s.x == 480) {
                source_texture_brique_bonus_s.x = 256;
            } else {
                source_texture_brique_bonus_s.x += 32;
            }
        }

        if(timer > 100) {
            timer = 0;
        }

        timer++;
    }
}