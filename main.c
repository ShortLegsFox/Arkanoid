#include <SDL2/SDL.h>
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


const int FPS = 60;
const int RECTIF = 5;
Uint64 precedent, maintenant; // Timers
double delta_temps;  // Durée frame en ms

void Initialise()
{
    Initialise_Fenetre();
    Initialise_Sprites();
    Charge_Niveau("../niveaux/niveau1.txt");
    Initialise_Balle();
    Initialise_Vaisseau();

    maintenant = SDL_GetPerformanceCounter();
}

void Dessine()
{
    Dessine_Fond();
    Dessine_Bordure();
    Dessine_Briques();

    Dessine_Vaisseau(x_pos_vaisseau, y_pos_vaisseau);
    Dessine_Balle(stats_balle.pos_x, stats_balle.pos_y);

    Met_A_Jour_Position_Balle();

    Gestion_Collision_Balle_Bord();
    Gestion_Collision_Balle_Haut();
    Gestion_Collision_Balle_Vaisseau();
    Gestion_Collision_Balle_Sortie_Bas();

    char* t_score = Entier_vers_Tableau(score_joueur);
    char* t_vies = Entier_vers_Tableau(vies);
    AfficheRectangleTextSprite("Score",10, 10);
    AfficheRectangleTextSprite(t_score, 10, 45);
    AfficheRectangleTextSprite("Vie", surface_fenetre->w - 65, 10);
    AfficheRectangleTextSprite(t_vies,surface_fenetre->w - 65, 45);

    if (vies < 0) Afficher_Game_Over();

    if(bonus_s) {
        Afficher_Bonus_S();
    }
    Met_A_Jour_Position_Bonus();
}

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
    {
        return 1;
    }

    Initialise();

    bool quitter = false;
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
            x_pos_vaisseau -= 10;
            if(x_pos_vaisseau < min_x) {
                x_pos_vaisseau = min_x;
            }
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            x_pos_vaisseau += 10;
            if(x_pos_vaisseau > max_x) {
                x_pos_vaisseau = max_x;
            }
        }
        if (keys[SDL_SCANCODE_ESCAPE])
            quitter=true;

        Collision_Balle_Brique();
        Dessine();

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

