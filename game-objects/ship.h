#include <stdbool.h>

#ifndef SHIP_H
#define SHIP_H

extern int x_pos_vaisseau; // Position horizontale du vaisseau
extern int y_pos_vaisseau; // Position verticale du vaisseau
// -- Permet d'éviter que la collision entre la balle et le vaisseau se fasse trop de fois en même temps causant ainsi un bug --
extern bool premiere_collision_vaisseau;

void Initialise_Vaisseau();

#endif