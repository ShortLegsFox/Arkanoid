#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int vies = 3;
const int FPS = 60;
const int RECTIF = 5;
struct { double pos_x; double pos_y;  double vitesse_x; double vitesse_y; } stats_balle; // On utilise un struct car il nous faut des doubles pour la précision des calculs
typedef struct { int pos_x; int pos_y; bool estBrique; } stats_brique; //
stats_brique briques[100][100];

Uint64 precedent, maintenant; // Timers
double delta_temps;  // Durée frame en ms
int x_pos_vaisseau; // Position horizontale du vaisseau

SDL_Window* pointeur_fenetre = NULL; // Pointeur vers la fenetre SDL
SDL_Surface* surface_fenetre = NULL; // Surface de la fenetre
SDL_Surface* textures_fenetre = NULL; // Planche de la texture de la fenetre
SDL_Surface* textures_objets = NULL; // Planche des textures des objets (briques)
SDL_Surface* textures_ascii = NULL; // Planche des textures des ASCII (aplphabet et chiffres)

// -- Découpage sur la planche de texture --
SDL_Rect source_texture_fond = {0, 128, 96, 128 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_balle = {0, 96, 24, 24 };
SDL_Rect source_texture_vaisseau = {128, 0, 128, 32 };
SDL_Rect source_texture_brique = { 0, 0, 30, 14 };

// -- Alphabet
SDL_Rect source_texture_A = {32, 70, 15, 19 };
SDL_Rect source_texture_V = {193, 102, 15, 22 };
SDL_Rect source_texture_i = {290, 131, 13, 22 };
SDL_Rect source_texture_e = {161, 139, 15, 20 };

// -- Chiffres
SDL_Rect source_texture_0 = {2, 38, 16, 22 };
SDL_Rect source_texture_1 = {34, 38, 16, 22 };
SDL_Rect source_texture_2 = {66, 38, 16, 22 };
SDL_Rect source_texture_3 = {98, 38, 13, 19 };
SDL_Rect source_texture_4 = {130, 38, 13, 19 };
SDL_Rect source_texture_5 = {162, 38, 13, 19 };
SDL_Rect source_texture_6 = {194, 38, 13, 19 };
SDL_Rect source_texture_7 = {226, 38, 13, 19 };
SDL_Rect source_texture_8 = {258, 38, 13, 19 };
SDL_Rect source_texture_9 = {290, 38, 13, 19 };

// -- Permet d'éviter que la collision entre la balle et le vaisseau se fasse trop de fois en même temps causant ainsi un bug --
bool premiere_collision_vaisseau = false;

void Recupere_Niveau(const char* nomFichier) {
    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier niveau");
        return;
    }

    char ligne[256];
    int largeurMax = 100;
    bool nextLine = false;

    int y = 0;
    int x = 0;
    while (fgets(ligne, sizeof(ligne), fichier)) {
        nextLine = false;
        while(!nextLine) {
            if (ligne[x] == '#') {
                briques[y][x].pos_x = x * 30;
                briques[y][x].pos_y = y * 14;
                briques[y][x].estBrique = true; // Brick
            } else {
                briques[y][x].pos_x = x * 30;
                briques[y][x].pos_y = y * 14;
                briques[y][x].estBrique = false; // Empty
            }

            if(ligne[x] == 'F') {
                y++;
                nextLine = true;
            }
            x++;
        }
        x = 0;
    }
}

void Initialise_Balle() {
    stats_balle.pos_x = surface_fenetre->w / 2;
    stats_balle.pos_y = surface_fenetre->h / 2;
    stats_balle.vitesse_x = 2.0;
    stats_balle.vitesse_y = 2.4;
}

void Initialise()
{
    pointeur_fenetre = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 600, 600, SDL_WINDOW_SHOWN);
    surface_fenetre = SDL_GetWindowSurface(pointeur_fenetre);
    textures_fenetre = SDL_LoadBMP("./sprites.bmp");
    textures_objets = SDL_LoadBMP("../assets/Arkanoid_sprites.bmp");
    textures_ascii = SDL_LoadBMP("../assets/Arkanoid_ascii.bmp");
    // Les parties de la textures qui sont noires deviennent transparentes
    SDL_SetColorKey(textures_fenetre, true, 0);
    SDL_SetColorKey(textures_ascii, true, 0);

    // Init les briques
    Recupere_Niveau("../niveaux/niveau1.txt");

    // balle (avec struct)
    Initialise_Balle();

    maintenant = SDL_GetPerformanceCounter();
}

void Deplace_Balle(){
    stats_balle.pos_x += stats_balle.vitesse_x;// / delta_t;
    stats_balle.pos_y += stats_balle.vitesse_y;// / delta_t;
}

//Casse les briques lors de la colision
void Collision_Balle_Brique() {
    SDL_Rect balleRect = { stats_balle.pos_x, stats_balle.pos_y, source_texture_balle.w, source_texture_balle.h };
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                SDL_Rect briqueRect = { briques[i][j].pos_x, briques[i][j].pos_y, source_texture_brique.w, source_texture_brique.h };
                if (SDL_HasIntersection(&balleRect, &briqueRect)) {
                    stats_balle.vitesse_y *= -1;
                    briques[i][j].estBrique = false;    // Marque la brique comme cassée
                    return;
                }
            }
        }
    }
}

// fonction qui met à jour la surface de la fenetre "win_surf"
void Dessine()
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
    SDL_BlitSurface(textures_fenetre, &source_texture_balle, surface_fenetre, &balle);

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
        stats_balle.vitesse_y *= -1;
        premiere_collision_vaisseau = true;
    }

    // Sortie par le bas
    if (stats_balle.pos_y > (surface_fenetre->h - balle.h)) {
        vies--;
        Initialise_Balle();
    }

    // Gestion vie
    SDL_Rect positionTexture0 = {10, 540, source_texture_0.w, source_texture_0.h};
    SDL_Rect positionTexture1 = {10, 540, source_texture_1.w, source_texture_1.h};
    SDL_Rect positionTexture2 = {10, 540, source_texture_2.w, source_texture_2.h};
    SDL_Rect positionTexture3 = {10, 540, source_texture_3.w, source_texture_3.h};

    SDL_Rect positionTextureV = {30, 540, source_texture_V.w, source_texture_V.h}; // Affiche V i e
    SDL_BlitSurface(textures_ascii,&source_texture_V, surface_fenetre, &positionTextureV);
    SDL_Rect positionTexturei = {50, 538, source_texture_i.w, source_texture_i.h};
    SDL_BlitSurface(textures_ascii,&source_texture_i, surface_fenetre, &positionTexturei);
    SDL_Rect positionTexturee = {70, 545, source_texture_e.w, source_texture_e.h};
    SDL_BlitSurface(textures_ascii,&source_texture_e, surface_fenetre, &positionTexturee);

    if (vies >= 0){
        switch (vies) {
            case 3:
                SDL_BlitSurface(textures_ascii,&source_texture_2, surface_fenetre, &positionTexture2);
                break;
            case 2:
                SDL_BlitSurface(textures_ascii,&source_texture_1, surface_fenetre, &positionTexture1);
                break;
            case 1:
                SDL_BlitSurface(textures_ascii,&source_texture_0, surface_fenetre, &positionTexture0);
                break;
            case 0:
                // todo Afficher un écran de game over
                SDL_Quit();
                break;
            default:
                SDL_Quit();
                break;
        }
    }

    // Touche le bas -> rouge
    if (premiere_collision_vaisseau)
        source_texture_balle.y = 64;
    // Touche le bas -> vert
    if (premiere_collision_vaisseau == false)
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

