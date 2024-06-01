#ifndef COLLIDER_H
#define COLLIDER_H
#include <stdbool.h>

void Gestion_Collision_Balle_Bord();
void Gestion_Collision_Balle_Haut();
void Gestion_Collision_Balle_Vaisseau();
void Gestion_Collision_Balle_Sortie_Bas();
void Gestion_Collision_Bonus_Vaisseau();
void Gestion_Collision_Bonus_Sortie_Bas();
void Collision_Balle_Briques(int i, int j);
bool Collision_Enemie_Brique(int index);
void Collision_Enemie_Bord(int index);
void Collision_Enemie_Balle(int index);
void Collision_Enemie_Vaisseau(int index);

#endif