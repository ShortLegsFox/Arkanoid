#include "ball.h"

#include <stdbool.h>

#include "ship.h"
#include "../rendering/rendering.h"

#define BALL_SPEED 5

bool balle_sur_vaisseau;
struct Balle stats_balle;

void Initialise_Balle() {
    stats_balle.pos_x = x_pos_vaisseau + 55;
    stats_balle.pos_y = y_pos_vaisseau - 15;
    stats_balle.vitesse_x = 5.0;
    stats_balle.vitesse_y = 5.5;
    balle_sur_vaisseau = true;
}

void Met_A_Jour_Position_Balle(){
    if (!balle_sur_vaisseau){
        stats_balle.pos_x += stats_balle.vitesse_x;// / delta_t;
        stats_balle.pos_y += stats_balle.vitesse_y;// / delta_t;
    } else {
        stats_balle.pos_x = x_pos_vaisseau + 55;
        stats_balle.pos_y = y_pos_vaisseau - 15;
    }
}

void Tirer_Balle() {
    if(balle_sur_vaisseau) {
        balle_sur_vaisseau = false;
    }
}