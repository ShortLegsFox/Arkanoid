#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "game-manager/game_manager.h"
#include "rendering/rendering.h"
#include "game-objects/ball.h"
#include "game-objects/bricks.h"
#include "game-objects/ship.h"
#include "game-objects/bonus.h"
#include "utils/utils.h"
#include "game-manager/collider.h"
#include "game-objects/enemy.h"
#include "game-objects/fireshot.h"
#include "game-objects/gates.h"


const int FPS = 60;
const int RECTIF = 5;
Uint64 precedent, maintenant; // Timers
double delta_temps;  // Durée frame en ms


void Initialise()
{
    Initialise_Fenetre();
    Initialise_Sprites();
    Charge_Niveau("../niveaux/niveau1.txt");
    Initialise_Portes();
    Initialise_Vaisseau();
    Initialise_Balle();
    Initialise_Enemie(0,1,'p');
    Initialise_Enemie(1,2,'c');

    maintenant = SDL_GetPerformanceCounter();
}

void Dessine()
{
    Dessine_Fond();
    Dessine_Bordure();
    Dessine_Briques();

    Dessine_Enemies(enemies, 2);

    Dessine_Vaisseau(x_pos_vaisseau, y_pos_vaisseau);
    Dessine_Balle(stats_balle.pos_x, stats_balle.pos_y);

    Met_A_Jour_Position_Balle();

    for(int i = 0; i < 10; i++) {
        if(objetsProjectiles[i].actif) {
            Met_A_Jour_Position_Projectile(&objetsProjectiles[i]);
            Dessine_Projectile(objetsProjectiles[i].pos_x, objetsProjectiles[i].pos_y);
            Verifie_Collision_Projectile_Brique(objetsProjectiles[i]);
        }
    }

    Met_A_Jour_Enemies();

    Gestion_Collision_Balle_Bord();
    Gestion_Collision_Balle_Haut();
    Gestion_Collision_Balle_Vaisseau();
    Gestion_Collision_Balle_Sortie_Bas();

    for(int i = 0; i< indexBonusDansTableau; i++) {
        Gestion_Collision_Bonus_Vaisseau(&objetBonus[i]);
        Gestion_Collision_Bonus_Sortie_Bas(&objetBonus[i]);
    }

    char* t_score = Entier_vers_Tableau(score_joueur);
    char* t_vies = Entier_vers_Tableau(vies);
    AfficheRectangleTextSprite("Score",10, 10);
    AfficheRectangleTextSprite(t_score, 10, 45);
    AfficheRectangleTextSprite("Vie", surface_fenetre->w - 65, 10);
    AfficheRectangleTextSprite(t_vies,surface_fenetre->w - 65, 45);

    if (vies < 0) Afficher_Game_Over();

    if(bonus_s) {
        //Afficher_Bonus_S();
        Initialise_Position_Bonus(source_texture_brique_bonus_s, 'S');
    }

    if(bonus_l) {
        //Afficher_Bonus_L();
        Initialise_Position_Bonus(source_texture_brique_bonus_l, 'L');
    }
    if(bonus_c) {
        Initialise_Position_Bonus(source_texture_brique_bonus_c, 'C');
    }

    Animation();
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    int a = 5;
    bool etatPrecedentS = false;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
    {
        return 1;
    }

    Initialise();

    bool quitter = false;
    bool etatPrecedentNiveau = false;
    SDL_Event evenement;
    while (!quitter)
    {
        while (SDL_PollEvent(&evenement))
        {
            if (evenement.type == SDL_QUIT)
            {
                quitter = true;
            }
        }

        SDL_PumpEvents();
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        int min_x =  0;
        int max_x = surface_fenetre->w - 128;

        if (keys[SDL_SCANCODE_LEFT]) {
            x_pos_vaisseau -= 15;
            if(x_pos_vaisseau < min_x) {
                x_pos_vaisseau = min_x;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            x_pos_vaisseau += 15;
            if(x_pos_vaisseau > max_x) {
                x_pos_vaisseau = max_x;
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE])
            quitter=true;

        if (keys[SDL_SCANCODE_SPACE])
            Tirer_Balle();

        bool etatActuelS = keys[SDL_SCANCODE_S];

        if(etatActuelS && !etatPrecedentS) {
            Initialise_Projectile();
        }

        etatPrecedentS = etatActuelS;

        bool etatNiveau = keys[SDL_SCANCODE_UP];
        if (etatNiveau && !etatPrecedentNiveau) {
            Passe_Niveau();
        }
        etatPrecedentNiveau = etatNiveau;

        Verifie_Collision_Balle_Brique();
        Dessine();

        Niveau_Suivant();

        SDL_UpdateWindowSurface(pointeur_fenetre);
        maintenant = SDL_GetPerformanceCounter();
        delta_temps = 1.0 / FPS - (double)(maintenant - precedent) / (double)SDL_GetPerformanceFrequency();
        precedent = maintenant;
        if (delta_temps > 0)
            SDL_Delay((Uint32)(delta_temps * 1000));
        //printf("dt = %lf\n", delta_temps * 1000);
        precedent = SDL_GetPerformanceCounter();
    }

    SDL_Quit();
    return 0;
}