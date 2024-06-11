#include "enemy.h"

#include <stdbool.h>
#include <time.h>

#include "gates.h"
#include "../game-manager/collider.h"
#include "../rendering/rendering.h"


Enemy enemies[10];
int timer_enemies = 0;
bool suiteAnimation = false;
int atom_y = 320;

void Initialise_Enemie(int index, int numero_porte, char type) {
    if(numero_porte == 1) {
        enemies[index].pos_x = porteUn.pos_x + porteUn.largeur / 4;
        enemies[index].pos_y = porteUn.pos_y + porteUn.hauteur;
    } else {
        enemies[index].pos_x = porteDeux.pos_x + porteDeux.largeur / 4;
        enemies[index].pos_y = porteDeux.pos_y + porteDeux.hauteur;
    }
    enemies[index].vitesse_x = 1.0;
    enemies[index].vitesse_y = 1.5;
    enemies[index].numero_porte = numero_porte;
    enemies[index].type = type;
    enemies[index].timer_animation = 0;
    enemies[index].estMort = false;
    enemies[index].timer_explosion = 0;
    enemies[index].explose = false;
    enemies[index].ligneAnimation = 0;
    texturePorteReset = true;
    timer_porte = 0;
}

void Animation_Enemie_Pyramide_Vert_Verre(int i) {
    if(timer_enemies % 10 == 0) {
        enemies[i].timer_animation = (enemies[i].timer_animation + 1) % 11;
    }

    if (enemies[i].type == 'p') {
        source_texture_pyramide_vert_verre.x = source_texture_pyramide_vert_verre.w * enemies[i].timer_animation;
    }
}

void Animation_Enemie_Chibre_Bleu(int i) {
    if(timer_enemies % 10 == 0) {
        enemies[i].timer_animation = (enemies[i].timer_animation + 1) % 8;
    }

    if (enemies[i].type == 'b') {
        source_texture_chibre_bleu.x = source_texture_chibre_bleu.w * enemies[i].timer_animation;
    }
}

void Animation_Enemie_Chromosome(int i) {
    if (timer_enemies % 10 == 0) {
        enemies[i].timer_animation++;

        if (enemies[i].timer_animation > 15) {
            enemies[i].timer_animation = 0;
            enemies[i].ligneAnimation++;
        }

        if (enemies[i].ligneAnimation == 1 && enemies[i].timer_animation > 7) {
            enemies[i].timer_animation = 0;
            enemies[i].ligneAnimation = 0;
        }
    }

    if (enemies[i].type == 'c') {
        source_texture_chromosome.x = source_texture_chromosome.w * enemies[i].timer_animation;
        source_texture_chromosome.y = atom_y + (source_texture_chromosome.h * enemies[i].ligneAnimation);
    }
}

void Animation_Explosion() {
    for(int i = 0; i < 2; i++) {
        if(enemies[i].estMort && enemies[i].explose) {
            Dessine_Texture(src_explosion, enemies[i].pos_x, enemies[i].pos_y);

            if(timer_enemies % 5 == 0) {
                src_explosion.x = enemies[i].timer_explosion*src_explosion.w;
                enemies[i].timer_explosion++;
            }

            if(enemies[i].timer_explosion > 5) {
                enemies[i].explose = false;
                enemies[i].timer_explosion = 0;
            }
        }
    }

}

void Chute_Enemies(int index) {
    if (!Collision_Enemie_Brique(index)) {
        if (enemies[index].pos_y > surface_fenetre->h / 2) {
            srand(time(NULL));
            enemies[index].pos_y += enemies[index].vitesse_y / 2;
            float amplitude = (rand() % 4)+1;
            float frequency = 0.05;
            enemies[index].pos_x += sin(enemies[index].pos_y * frequency) * amplitude;
        } else {
            enemies[index].pos_y += enemies[index].vitesse_y;
        }
    }
}

void Deplacement_Enemies(int index) {
    if(Collision_Enemie_Brique(index)) {
        enemies[index].pos_x += enemies[index].vitesse_x;
    }
}

void Met_A_Jour_Enemies() {
    timer_enemies++;
    for (int i = 0; i < 2; i++) {
        if (!enemies[i].estMort) {
            if (enemies[i].type == 'p') {
                Animation_Enemie_Pyramide_Vert_Verre(i);
            } else if (enemies[i].type == 'b') {
                Animation_Enemie_Chibre_Bleu(i);
            } else if (enemies[i].type == 'c') {
                Animation_Enemie_Chromosome(i);
            }
            Chute_Enemies(i);
            Deplacement_Enemies(i);
            Collision_Enemie_Bord(i);
            Collision_Enemie_Balle(i);
            Collision_Enemie_Vaisseau(i);
            Gestion_Collision_Enemie_Sortie_Bas(i);
        } else if(!enemies[i].explose && enemies[i].estMort) {
            int numero_porte = rand() % 2;
            Initialise_Enemie(i,numero_porte,Enemie_Aleatoire());
            texturePorteReset = true;
        }
    }
}

char Enemie_Aleatoire() {
    srand(time(NULL));
    int value = (rand() % 3);

    if(value == 0) {
        return 'c';
    }

    if(value == 1) {
        return 'b';
    }

    return 'p';
}
