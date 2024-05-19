#include "rendering.h"
#include "../utils/utils.h"
#include "../game-manager/game_manager.h"
#include "../game-objects/bricks.h"

SDL_Window* pointeur_fenetre = NULL; // Pointeur vers la fenetre SDL
SDL_Surface* surface_fenetre = NULL; // Surface de la fenetre
SDL_Surface* texture_noir = NULL; // Planche de la texture de la fenetre
SDL_Surface* textures_objets = NULL; // Planche des textures des objets (briques)
SDL_Surface* textures_ascii = NULL; // Planche des textures des ASCII (aplphabet et chiffres)
SDL_Surface* textures_gameover = NULL; // Planche des textures game over

// -- Découpage sur la planche de texture --
SDL_Rect source_texture_fond = {240, 128, 48, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_sombre = {240, 192, 48, 64 }; // Le point (0,0) est en haut a gauche

// -- Texture de fond noir --
SDL_Rect source_texture_noir = {96, 128, 48, 64};

SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste = {320, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_rouge = {320, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_ambiance_fossoyeuse = {320, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_bleu = {320, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_defaut = {320, 128, 64, 64 }; // Le point (0,0) est en haut a gauche

// -- Taille de la balle - Pourrait être automatisé ? --
SDL_Rect source_texture_balle_xs = {0, 64, 16, 16 };
SDL_Rect source_texture_balle_s = {16, 64, 16, 16 };
SDL_Rect source_texture_balle_m = {32, 64, 16, 16 };
SDL_Rect source_texture_balle_l = {48, 64, 16, 16 };
SDL_Rect source_texture_balle_xl = {64, 64, 16, 16 };
SDL_Rect source_texture_balle_2xl = {80, 64, 16, 16 };

SDL_Rect source_texture_balle = {80, 64, 16, 16 };

// -- Taille du vaisseau - idem que la balle ? --
SDL_Rect source_texture_vaisseau = {384, 303, 128, 16 };

SDL_Rect source_texture_brique = { 0, 0, 32, 16 };

// -- Bonus S --
SDL_Rect source_texture_brique_bonus_s = { 256, 0, 32, 16 };
// -- Bonus C --
SDL_Rect source_texture_brique_bonus_c = { 256, 16, 32, 16 };

// -- Game over
SDL_Rect source_texture_gameover = {0, 0, 558, 518};

// -- Bordure du jeu
SDL_Rect src_bordure_verticale = {41, 100, 15, 15};
SDL_Rect src_bordure_verticale_porte = {448, 120, 15, 75};
SDL_Rect src_bordure_horizontale = {57, 85, 15, 15};
SDL_Rect src_bordure_coin_gauche = {41, 85, 15, 15};
SDL_Rect src_bordure_coin_droit = {73, 85, 15, 15};
SDL_Rect source_texture_bordure_porte_horizontale = {298, 127, 48, 18};

int topMargin = 100;

void Initialise_Fenetre() {
    // Taille de la fenêtre
    pointeur_fenetre = SDL_CreateWindow("Arknoid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 576, 740, SDL_WINDOW_SHOWN);
    surface_fenetre = SDL_GetWindowSurface(pointeur_fenetre);
}
void Initialise_Sprites() {
    // Chargement des sprites
    texture_noir = SDL_LoadBMP("./assets/sprites.bmp");
    textures_objets = SDL_LoadBMP("./assets/sprites2.bmp");
    textures_ascii = SDL_LoadBMP("./assets/Arkanoid_ascii.bmp");
    textures_gameover = SDL_LoadBMP("./assets/gameover.bmp");

    // Partie noire du sprite en transparence, sauf pour celui où l'on veut recuperer le noir
    SDL_SetColorKey(textures_ascii, true, 0);
    SDL_SetColorKey(textures_gameover, true, 0);
    SDL_SetColorKey(textures_objets, true, 0);
}

void Dessine_Texture(SDL_Rect texture, int x, int y) {
    SDL_Rect curseur_texture = {x, y, 0, 0 };
    SDL_BlitSurface(textures_objets, &texture, surface_fenetre, &curseur_texture);
}

void Dessine_Noir(int x, int y) {
    SDL_Rect curseur_texture = {x, y, 0, 0 };
    SDL_BlitSurface(texture_noir, &source_texture_noir, surface_fenetre, &curseur_texture);
}

void Dessine_Fond() {
    // Partie noir en haut
    for (int j = 0; j < topMargin; j += source_texture_fond.h) {
        for (int i = 0; i < surface_fenetre->w; i += source_texture_fond.w) {
            Dessine_Noir(i,j);
        }
    }

    // Partie jeu
    bool premiereLigne = true;
    for (int j = topMargin; j < surface_fenetre->h; j += source_texture_fond.h) {
        for (int i = 0; i < surface_fenetre->w; i += source_texture_fond.w) {
            if(i == 0 || premiereLigne) {
                Dessine_Texture(source_texture_fond_sombre ,i, j);
            }
            else {
                Dessine_Texture(source_texture_fond, i, j);
            }
        }
        if (premiereLigne) premiereLigne = false;
    }
}

void Dessine_Bordure() {
    int dernierAbscisse = surface_fenetre->w - src_bordure_coin_droit.w;

    // Bord Coin Gauche
    Dessine_Texture(src_bordure_coin_gauche, 0, topMargin);
    SDL_Rect curseur_bordure = {0, topMargin, 0, 0 };

    // Bord Haut
    for (int i = src_bordure_coin_gauche.w; i < surface_fenetre->w - src_bordure_coin_droit.w; i += src_bordure_horizontale.w) {
        curseur_bordure.x = i;
        Dessine_Texture(src_bordure_horizontale, i, topMargin);
    }

    // Bord Coin Droit
    curseur_bordure.x = dernierAbscisse;
    SDL_BlitSurface(textures_objets, &src_bordure_coin_droit, surface_fenetre, &curseur_bordure);

    // Bord Cotes Porte
    int occurence = 1; // Ajustement pour que la porte soit au niveau du vaisseau, cela rajoute juste un petit mur vertical en plus après les coins supérieurs
    int compteur = 0;
    for(int j = src_bordure_coin_gauche.h + topMargin; j < surface_fenetre->h; j += src_bordure_verticale.h) {
        if (compteur > occurence) {
            Dessine_Texture(src_bordure_verticale_porte, 0, j);
            Dessine_Texture(src_bordure_verticale_porte, dernierAbscisse, j);
            j += src_bordure_verticale_porte.h - src_bordure_verticale.h;
        } else {
            Dessine_Texture(src_bordure_verticale, 0, j);
            Dessine_Texture(src_bordure_verticale, dernierAbscisse, j);
            compteur++;
        }
    }
}

void Dessine_Briques() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                Dessine_Texture(source_texture_brique, briques[i][j].pos_x, briques[i][j].pos_y);
            }
        }
    }
}

void Dessine_Vaisseau(int x, int y) {
    Dessine_Texture(source_texture_vaisseau, x, y);
}

void Dessine_Balle(int x, int y) {
    Dessine_Texture(source_texture_balle, x, y);
}

void CalculRectangleCaractereSprite(char character, SDL_Rect* sourceRect, int spriteWidth, int spriteHeight, int charsPerLine) {
    int ajustementDepart = 32;
    int asciiValue = (int)character;
    int charIndex = asciiValue - ajustementDepart;

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

void AfficheRectangleTextSprite(char text[], int coord_x, int coord_y) {
    int text_length = strlen(text);
    int x = coord_x;
    int y = coord_y;

    for(int i = 0; i < text_length; i++) {
        AfficheRectangleCaractereSprite(text[i], x, y);
        x += 17;
    }
}

void Afficher_Game_Over()
{
    // Fond noir
    SDL_FillRect(surface_fenetre, NULL, SDL_MapRGB(surface_fenetre->format, 0, 0, 0));

    // Afficher Game Over t
    SDL_Rect gameover = {0, 0, source_texture_gameover.w, source_texture_gameover.h};
    SDL_BlitSurface(textures_gameover, &source_texture_gameover, surface_fenetre, &gameover);

    char* t_score = Entier_vers_Tableau(score_joueur);
    AfficheRectangleTextSprite("Score", 190, 10);
    AfficheRectangleTextSprite(t_score, 320, 10);

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