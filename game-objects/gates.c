#include "gates.h"

#include "../rendering/rendering.h"

struct Porte porteUn;
struct Porte porteDeux;

void Initialise_Portes() {
    porteUn.pos_x = 120;
    porteUn.pos_y = topMargin - 5;
    porteUn.hauteur = source_texture_bordure_porte_horizontale.h;
    porteUn.largeur = source_texture_bordure_porte_horizontale.w;

    porteDeux.pos_x = 391;
    porteDeux.pos_y = topMargin - 5;
    porteDeux.hauteur = source_texture_bordure_porte_horizontale.h;
    porteDeux.largeur = source_texture_bordure_porte_horizontale.w;
}

