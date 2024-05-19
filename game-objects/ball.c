#include "ball.h"
#include "../rendering/rendering.h"

struct Balle stats_balle;

void Initialise_Balle() {
    stats_balle.pos_x = surface_fenetre->w / 2.0;
    stats_balle.pos_y = surface_fenetre->h / 2.0;
    stats_balle.vitesse_x = 2.0;
    stats_balle.vitesse_y = 2.4;
}

void Met_A_Jour_Position_Balle(){
    stats_balle.pos_x += stats_balle.vitesse_x;// / delta_t;
    stats_balle.pos_y += stats_balle.vitesse_y;// / delta_t;
}