#include "ball.h"
#include "../rendering/rendering.h"

#define BALL_SPEED 5

struct Balle stats_balle;

void Initialise_Balle() {
    stats_balle.pos_x = surface_fenetre->w / 2.0;
    stats_balle.pos_y = surface_fenetre->h - surface_fenetre->h / 3.0;
    stats_balle.vitesse_x = 2.0;
    stats_balle.vitesse_y = 2.0;
}

void Met_A_Jour_Position_Balle(){
    stats_balle.pos_x += stats_balle.vitesse_x;// / delta_t;
    stats_balle.pos_y += stats_balle.vitesse_y;// / delta_t;
}