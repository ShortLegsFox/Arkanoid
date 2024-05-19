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

// fonction qui met à jour la surface de la fenetre "win_surf"
void Dessine()
{
    Dessine_Fond();
    Dessine_Bordure();
    Dessine_Briques();

    Dessine_Vaisseau(x_pos_vaisseau, y_pos_vaisseau);
    Dessine_Balle(stats_balle.pos_x, stats_balle.pos_y);

    // Entitées dessinées
    SDL_Rect vaisseau = {x_pos_vaisseau, surface_fenetre->h - 40, source_texture_vaisseau.w, source_texture_vaisseau.h};
    SDL_Rect balle = {stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h};

    Deplace_Balle();

    // Collision bord
    if ((stats_balle.pos_x < 15) || (stats_balle.pos_x > (surface_fenetre->w - balle.w - 15))) {
        stats_balle.vitesse_x *= -1;
        premiere_collision_vaisseau = false;
    }
    if ((stats_balle.pos_y < 15 + topMargin)) {
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = false;
    }

    // Collision vaisseau
    if ((SDL_HasIntersection(&balle, &vaisseau)) && premiere_collision_vaisseau == false){
        int collisionX = balle.x + balle.w / 2; // Position x de la collision
        int vaisseauX = vaisseau.x + vaisseau.w / 2; // Position x du centre du vaisseau

        if (collisionX < vaisseauX) {
            stats_balle.vitesse_x = -abs(stats_balle.vitesse_x);
        } else {
            stats_balle.vitesse_x = abs(stats_balle.vitesse_x);
        }
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = true;
    }

    // Sortie par le bas
    if (stats_balle.pos_y > (surface_fenetre->h - balle.h)) {
        vies--;
        Initialise_Balle();
    }

    char* t_score = Entier_vers_Tableau(score_joueur);
    char* t_vies = Entier_vers_Tableau(vies);
    AfficheRectangleTextSprite("Score",10, 10);
    AfficheRectangleTextSprite(t_score, 10, 45);
    AfficheRectangleTextSprite("Vie", surface_fenetre->w - 65, 10);
    AfficheRectangleTextSprite(t_vies,surface_fenetre->w - 65, 45);

    if (vies < 0) Afficher_Game_Over();

    // Spawn le bonus S
    if(bonus_s) {
        SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w, source_texture_brique_bonus_s.h};
        int i = coord_x_brique_cassee;
        int j = coord_y_brique_cassee;
        Initialise_Bonus(briques[i][j].pos_x,briques[i][j].pos_y);
        SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);
    }
    Tomber_Bonus();
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

