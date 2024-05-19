#ifndef SOLID_BRICK_H
#define SOLID_BRICK_H

#include <stdbool.h>

typedef struct {
    int pos_x;
    int pos_y;
    bool estBrique;
} stats_brique_solide;

extern stats_brique_solide briques[100][100];

#endif
