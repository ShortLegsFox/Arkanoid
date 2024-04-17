#ifndef BRICKS_H
#define BRICKS_H

#include <stdbool.h>

typedef struct {
    int pos_x;
    int pos_y;
    bool estBrique;
} stats_brique;

extern stats_brique briques[100][100];

#endif