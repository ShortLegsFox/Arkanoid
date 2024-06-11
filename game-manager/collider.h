#ifndef COLLIDER_H
#define COLLIDER_H
#include <stdbool.h>
#include "../game-objects/bonus.h"
#include "../game-objects/fireshot.h"

void Gestion_Collision_Balle_Bord();
void Gestion_Collision_Balle_Haut();
void Gestion_Collision_Balle_Vaisseau();
void Gestion_Collision_Balle_Sortie_Bas();
void Gestion_Collision_Bonus_Vaisseau(struct Bonus *objetBonus);
void Gestion_Collision_Bonus_Sortie_Bas(struct Bonus *objetBonus);
void Collision_Balle_Briques(int i, int j);
void Collision_Projectile_Briques(struct Projectile * projectile, int i, int j);
void Gestion_Collision_Projectile_Haut(struct Projectile * projectile);
void Collision_Enemie_Projectile(int index, struct Projectile * projectile);
bool Collision_Enemie_Brique(int index);
void Collision_Enemie_Bord(int index);
void Collision_Enemie_Balle(int index);
void Collision_Enemie_Vaisseau(int index);
void Gestion_Collision_Enemie_Sortie_Bas(int index);

#endif