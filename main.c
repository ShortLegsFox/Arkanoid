#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int FPS=60.0;
const int RECTIF = 5;
struct { double pos_x; double pos_y;  double vitesse_x; double vitesse_y; } stats_balle; // On utilise un struct car il nous faut des doubles pour la précision des calculs

Uint64 prev, now; // Timers
double delta_t;  // Durée frame en ms
int x_pos_vaisseau; // Position horizontale du vaisseau

SDL_Window* pointeur_fenetre = NULL; // Pointeur vers la fenetre SDL
SDL_Surface* surface_fenetre = NULL; // Surface de la fenetre
SDL_Surface* textures_fenetre = NULL; // Planche de la texture de la fenetre

// -- Découpage sur la planche de texture --
SDL_Rect source_texture_fond = {0, 128, 96, 128 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_balle = {0, 96, 24, 24 };
SDL_Rect source_texture_vaisseau = {128, 0, 128, 32 };

// -- Permet d'éviter que la collision entre la balle et le vaisseau se fasse trop de fois en même temps causant ainsi un bug
bool premiere_collision_vaisseau = false;


void init_balle() {
    stats_balle.pos_x = surface_fenetre->w / 2;
    stats_balle.pos_y = surface_fenetre->h / 2;
    stats_balle.vitesse_x = 2.0;
    stats_balle.vitesse_y = 2.4;
}

void init()
{
    pointeur_fenetre = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
    surface_fenetre = SDL_GetWindowSurface(pointeur_fenetre);
    textures_fenetre = SDL_LoadBMP("./sprites.bmp");
    // Les parties de la textures qui sont noires deviennent transparentes
    SDL_SetColorKey(textures_fenetre, true, 0);

    // balle (avec struct)
    init_balle();

    now = SDL_GetPerformanceCounter();
}

// fonction qui met à jour la surface de la fenetre "win_surf"
void draw()
{
    // remplit le fond
    SDL_Rect curseur_texture = {0, 0, 0, 0 };
    for (int j = 0; j < surface_fenetre->h; j+=128)
        for (int i = 0; i < surface_fenetre->w; i += 96)
        {
            curseur_texture.x = i;
            curseur_texture.y = j;
            SDL_BlitSurface(textures_fenetre, &source_texture_fond, surface_fenetre, &curseur_texture);
        }

    // Entitées dessinées
    SDL_Rect vaisseau = {x_pos_vaisseau, surface_fenetre->h - 32, source_texture_vaisseau.w, source_texture_vaisseau.h};
    SDL_Rect balle = {stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h};

    // Afficher balle
    SDL_BlitSurface(textures_fenetre, &source_texture_balle, surface_fenetre, &balle);

    // Dédplacement
    stats_balle.pos_x += stats_balle.vitesse_x;// / delta_t;
    stats_balle.pos_y += stats_balle.vitesse_y;// / delta_t;

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
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = true;
    }

    // Sortie par le bas
    if (stats_balle.pos_y > (surface_fenetre->h - balle.h - RECTIF)) {
        init_balle();
    }

    // touche bas -> rouge
    if (stats_balle.pos_y > (surface_fenetre->h - balle.h))
        source_texture_balle.y = 64;
    // touche bas -> vert
    if (stats_balle.pos_y < 1)
        source_texture_balle.y = 96;

    // Afficher vaisseau
    curseur_texture.x = x_pos_vaisseau;
    curseur_texture.y = surface_fenetre->h - vaisseau.h;
    SDL_BlitSurface(textures_fenetre, &source_texture_vaisseau, surface_fenetre, &curseur_texture);
}

int main(int argc, char** argv)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
    {
        return 1;
    }

    init();

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

        draw();

        SDL_UpdateWindowSurface(pointeur_fenetre);
        now = SDL_GetPerformanceCounter();
        delta_t = 1.0/FPS - (double)(now - prev) / (double)SDL_GetPerformanceFrequency();
        prev = now;
        if (delta_t > 0)
            SDL_Delay((Uint32)(delta_t*1000));
        printf("dt = %lf\n",delta_t*1000);
        prev = SDL_GetPerformanceCounter();
    }

    SDL_Quit();
    return 0;
}

