#include <stdbool.h>

#ifndef SHIP_H
#define SHIP_H

int x_pos_vaisseau; // Position horizontale du vaisseau
// -- Permet d'éviter que la collision entre la balle et le vaisseau se fasse trop de fois en même temps causant ainsi un bug --
bool premiere_collision_vaisseau = false;

#endif