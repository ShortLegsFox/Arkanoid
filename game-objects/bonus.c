#include "bonus.h"
#include "../rendering/rendering.h"

struct Bonus stats_bonus;
bool bonus_en_cours;

void Initialise_Bonus(int coord_x, int coord_y) {
    stats_bonus.pos_x = coord_x;
    stats_bonus.pos_y = coord_y;
    stats_bonus.vitesse_x = 2.0;
    stats_bonus.vitesse_y = 2.4;
    bonus_en_cours = true;
}

void Tomber_Bonus() {
    if(bonus_en_cours) {
        stats_bonus.pos_y += stats_bonus.vitesse_y;// / delta_t;
        SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w,
                          source_texture_brique_bonus_s.h};
        SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);
    }
}