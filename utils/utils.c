#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

#include <math.h>

char* Entier_vers_Tableau(int entier) {
    char* t_entier = (char*)malloc(12 * sizeof(char));
    if (t_entier == NULL) {
        return NULL;
    }
    sprintf(t_entier, "%d", entier);
    return t_entier;
}

float Racine_Carree(float nombre) {
    return pow(nombre, 0.5);
}