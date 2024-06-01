#ifndef GATES_H
#define GATES_H

struct Porte {
    int pos_x;
    int pos_y;
    int hauteur;
    int largeur;
};

extern struct Porte porteUn;
extern struct Porte porteDeux;

void Initialise_Portes();

#endif
