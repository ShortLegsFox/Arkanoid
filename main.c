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


const int FPS = 60;
const int RECTIF = 5;

Uint64 precedent, maintenant; // Timers
double delta_temps;  // Durée frame en ms

void Initialise()
{
    pointeur_fenetre = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
    surface_fenetre = SDL_GetWindowSurface(pointeur_fenetre);
    textures_fenetre = SDL_LoadBMP("./assets/sprites.bmp");
    textures_objets = SDL_LoadBMP("./assets/Arkanoid_sprites.bmp");
    textures_ascii = SDL_LoadBMP("./assets/Arkanoid_ascii.bmp");
    textures_gameover = SDL_LoadBMP("./assets/gameover.bmp");
    // Les parties de la textures qui sont noires deviennent transparentes
    SDL_SetColorKey(textures_fenetre, true, 0);
    SDL_SetColorKey(textures_ascii, true, 0);
    SDL_SetColorKey(textures_gameover, true, 0);
    SDL_SetColorKey(textures_objets, true, 0);

    // Init les briques
    Recupere_Niveau("../niveaux/niveau1.txt");

    // balle (avec struct)
    Initialise_Balle();

    maintenant = SDL_GetPerformanceCounter();
}

// fonction qui met à jour la surface de la fenetre "win_surf"
void Dessine()
{
    // remplit le fond
    SDL_Rect curseur_texture = {0, 0, 0, 0 };
    for (int j = 0; j < surface_fenetre->h; j+=64)
        for (int i = 0; i < surface_fenetre->w; i += 64)
        {
            curseur_texture.x = i;
            curseur_texture.y = j;
            SDL_BlitSurface(textures_objets, &source_texture_fond, surface_fenetre, &curseur_texture);
        }

    // remplit les briques
    SDL_Rect curseur_texture_briques = {0, 0, 0, 0 };
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                curseur_texture_briques.x = briques[i][j].pos_x;
                curseur_texture_briques.y = briques[i][j].pos_y;
                SDL_BlitSurface(textures_objets, &source_texture_brique, surface_fenetre, &curseur_texture_briques);
            }
        }
    }

    // Entitées dessinées
    SDL_Rect vaisseau = {x_pos_vaisseau, surface_fenetre->h - 32, source_texture_vaisseau.w, source_texture_vaisseau.h};
    SDL_Rect balle = {stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h};

    // Afficher balle
    SDL_BlitSurface(textures_objets, &source_texture_balle, surface_fenetre, &balle);

    Deplace_Balle();

    // Collision bord
    if ((stats_balle.pos_x < 1) || (stats_balle.pos_x > (surface_fenetre->w - balle.w))) {
        stats_balle.vitesse_x *= -1;
        premiere_collision_vaisseau = false;
    }
    if ((stats_balle.pos_y < 1)) {
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

    AfficheRectangleCaractereSprite('S', 10, 10);
    AfficheRectangleCaractereSprite('c', 30, 10);
    AfficheRectangleCaractereSprite('o', 50, 10);
    AfficheRectangleCaractereSprite('r', 70, 10);
    AfficheRectangleCaractereSprite('e', 90, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_dix_milliers, 130, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_milliers, 150, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_centaine, 170, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_dizaine, 190, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_unite, 210, 10);

    AfficheRectangleCaractereSprite('V', 500, 10);
    AfficheRectangleCaractereSprite('i', 520, 10);
    AfficheRectangleCaractereSprite('e', 540, 10);

    if (vies >= 0){
        SDL_Rect source_texture_nombre_vies = {};
        CalculRectangleCaractereSprite('0' + vies, &source_texture_nombre_vies, 32, 32, 16);
        SDL_Rect positionTextureVies = {570, 10, source_texture_nombre_vies.w, source_texture_nombre_vies.h};
        SDL_BlitSurface(textures_ascii,&source_texture_nombre_vies, surface_fenetre, &positionTextureVies);
    }
    else {
        Afficher_Game_Over();
    }

    // Afficher vaisseau
    curseur_texture.x = x_pos_vaisseau;
    curseur_texture.y = surface_fenetre->h - vaisseau.h;
    SDL_BlitSurface(textures_objets, &source_texture_vaisseau, surface_fenetre, &vaisseau);

    // Spawn la brique
    if(bonus_s) {
        SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w, source_texture_brique_bonus_s.h};
        int i = coord_x_brique_cassee;
        int j = coord_y_brique_cassee;
        Initialise_Bonus(briques[i][j].pos_x,briques[i][j].pos_y);
        SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);
    }

    // Glisse la brique vers le bas
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
        printf("dt = %lf\n", delta_temps * 1000);
        precedent = SDL_GetPerformanceCounter();
    }

    SDL_Quit();
    return 0;
}

