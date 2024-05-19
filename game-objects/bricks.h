#ifndef BRICKS_H
#define BRICKS_H

#include <stdbool.h>

typedef struct {
    int pos_x;
    int pos_y;
    int code_couleur;
    bool estBrique;
    bool estSolide;
} stats_brique;

extern stats_brique briques[100][100];

void Initialise_Brique(int x, int y, int code_couleur);
void Initialise_Brique_Solide(int x, int y, int code_couleur);
void Initialise_Position_Vide(int x, int y);

#endif