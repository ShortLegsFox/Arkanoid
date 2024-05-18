#include "ship.h"
#include "../rendering/rendering.h"

int x_pos_vaisseau;
int y_pos_vaisseau;
bool premiere_collision_vaisseau = false;

void Initialise_Vaisseau() {
    x_pos_vaisseau = surface_fenetre->w / 2 - source_texture_vaisseau.w / 2;
    y_pos_vaisseau = surface_fenetre->h - 40;
}