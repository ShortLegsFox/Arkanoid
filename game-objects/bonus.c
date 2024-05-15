#include "bonus.h"
#include "../rendering/rendering.h"

struct Bonus stats_bonus;

void Initialise_Bonus(int coord_x, int coord_y) {
    stats_bonus.pos_x = coord_x;
    stats_bonus.pos_y = coord_y;
    stats_bonus.vitesse_x = 2.0;
    stats_bonus.vitesse_y = 2.4;
}

void Tomber_Bonus() {
    stats_bonus.pos_y += stats_bonus.vitesse_y;// / delta_t;
    SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w, source_texture_brique_bonus_s.h};
    SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);
}