#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int vies = 2;
int score_joueur_unite = 0;
int score_joueur_dizaine = 0;
int score_joueur_centaine = 0;
int score_joueur_milliers = 0;
int score_joueur_dix_milliers = 0;
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
SDL_Surface* textures_gameover = NULL; // Planche des textures game over

// -- Découpage sur la planche de texture --
SDL_Rect source_texture_fond = {0, 128, 96, 128 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_balle = {0, 96, 24, 24 };
SDL_Rect source_texture_vaisseau = {128, 0, 128, 32 };
SDL_Rect source_texture_brique = { 0, 0, 30, 14 };

// -- Game over
SDL_Rect source_texture_gameover = {0, 0, 558, 518};

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
    textures_fenetre = SDL_LoadBMP("./assets/sprites.bmp");
    textures_objets = SDL_LoadBMP("./assets/Arkanoid_sprites.bmp");
    textures_ascii = SDL_LoadBMP("./assets/Arkanoid_ascii.bmp");
    textures_gameover = SDL_LoadBMP("./assets/gameover.bmp");
    // Les parties de la textures qui sont noires deviennent transparentes
    SDL_SetColorKey(textures_fenetre, true, 0);
    SDL_SetColorKey(textures_ascii, true, 0);
    SDL_SetColorKey(textures_gameover, true, 0);

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

void Ajuster_Score() {
    if (score_joueur_unite > 9) {
        score_joueur_unite = 0;
        score_joueur_dizaine ++;
    }
    if (score_joueur_dizaine > 9) {
        score_joueur_dizaine= 0;
        score_joueur_centaine ++;
    }
    if (score_joueur_centaine > 9) {
        score_joueur_centaine = 0;
        score_joueur_milliers ++;
    }
    if (score_joueur_milliers > 9) {
        score_joueur_milliers = 0;
        score_joueur_dix_milliers ++;
    }
}

void CalculRectangleCaractereSprite(char character, SDL_Rect* sourceRect, int spriteWidth, int spriteHeight, int charsPerLine) {
    // Assuming the sprite starts with a space character (ASCII 32)
    int asciiValue = (int)character;
    int charIndex = asciiValue - 32; // Adjust for the starting character

    int row = charIndex / charsPerLine;
    int col = charIndex % charsPerLine;

    sourceRect->x = col * spriteWidth;
    sourceRect->y = row * spriteHeight;
    sourceRect->w = spriteWidth;
    sourceRect->h = spriteHeight;
}

void AfficheRectangleCaractereSprite(char character, int coord_x, int coord_y) {
    SDL_Rect source_rect = {};
    CalculRectangleCaractereSprite(character, &source_rect, 32, 32, 16);
    SDL_Rect position_rect = {coord_x, coord_y, source_rect.w, source_rect.h};
    SDL_BlitSurface(textures_ascii,&source_rect, surface_fenetre, &position_rect);
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
                    score_joueur_unite++;
                    Ajuster_Score();
                    return;
                }
            }
        }
    }
}

void Afficher_Game_Over()
{
    // Fond noir
    SDL_FillRect(surface_fenetre, NULL, SDL_MapRGB(surface_fenetre->format, 0, 0, 0));

    // Afficher Game Over t
    SDL_Rect gameover = {0, 0, source_texture_gameover.w, source_texture_gameover.h};
    SDL_BlitSurface(textures_gameover, &source_texture_gameover, surface_fenetre, &gameover);

    AfficheRectangleCaractereSprite('S', 190, 10);
    AfficheRectangleCaractereSprite('c', 210, 10);
    AfficheRectangleCaractereSprite('o', 230, 10);
    AfficheRectangleCaractereSprite('r', 250, 10);
    AfficheRectangleCaractereSprite('e', 270, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_dix_milliers, 300, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_milliers, 320, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_centaine, 340, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_dizaine, 360, 10);
    AfficheRectangleCaractereSprite('0'+score_joueur_unite, 380, 10);

    SDL_UpdateWindowSurface(pointeur_fenetre);

    // Attendre une entrée utilisateur avant de quitter
    SDL_Event evenement;
    bool attend = true;
    while (attend) {
        while (SDL_PollEvent(&evenement)) {
            if (evenement.type == SDL_KEYDOWN) { // Attend que l'utilisateur appuie sur une touche (bouton power)
                attend = false;
            }
            if (evenement.type == SDL_QUIT) {
                attend = false;
            }
        }
    }

    SDL_Quit();
    exit(0);
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

