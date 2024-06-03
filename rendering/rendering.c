#include "rendering.h"
#include <time.h>

#include "../utils/utils.h"
#include "../game-manager/game_manager.h"
#include "../game-objects/bricks.h"
#include "../game-objects/bonus.h"
#include "../game-objects/gates.h"
#include "../game-objects/enemy.h"

SDL_Window* pointeur_fenetre = NULL; // Pointeur vers la fenetre SDL
SDL_Surface* surface_fenetre = NULL; // Surface de la fenetre
SDL_Surface* texture_noir = NULL; // Planche de la texture de la fenetre
SDL_Surface* textures_objets = NULL; // Planche des textures des objets (briques)
SDL_Surface* textures_ascii = NULL; // Planche des textures des ASCII (aplphabet et chiffres)
SDL_Surface* textures_gameover = NULL; // Planche des textures game over
SDL_Surface* textures_2 = NULL; // Planche des textures game over


// -- Découpage sur la planche de texture --
SDL_Rect source_texture_fond = {0, 128, 48, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_sombre = {0, 192, 48, 64 }; // Le point (0,0) est en haut a gauche

// -- Texture de fond noir --
SDL_Rect source_texture_noir = {96, 128, 48, 64};

// -- Autres textures
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste = {240, 128, 48, 64  }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_mecanique_en_feu = {192, 128, 48, 64  }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_rouge = {144, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_bleu = {96, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_ambiance_fossoyeuse = {48, 128, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_defaut = {0, 128, 64, 64 }; // Le point (0,0) est en haut a gauche

// -- Autres textures sombre
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_sombre = {240, 192, 48, 64  }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_mecanique_en_feu_sombre = {192, 192, 48, 64  }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_rouge_sombre = {144, 192, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_circuit_imprime_retro_futuriste_bleu_sombre = {96, 192, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_ambiance_fossoyeuse_sombre = {48, 192, 64, 64 }; // Le point (0,0) est en haut a gauche
SDL_Rect source_texture_fond_defaut_sombre = {0, 192, 64, 64 }; // Le point (0,0) est en haut a gauche

// -- Taille de la balle - Pourrait être automatisé ? --
SDL_Rect source_texture_balle_xs = {0, 64, 16, 16 };
SDL_Rect source_texture_balle_s = {16, 64, 16, 16 };
SDL_Rect source_texture_balle_m = {32, 64, 16, 16 };
SDL_Rect source_texture_balle_l = {48, 64, 16, 16 };
SDL_Rect source_texture_balle_xl = {64, 64, 16, 16 };
SDL_Rect source_texture_balle_2xl = {80, 64, 16, 16 };

SDL_Rect source_texture_balle = {80, 65, 16, 16 };

// -- Taille du vaisseau - idem que la balle ? --
SDL_Rect source_texture_vaisseau = {384, 303, 128, 16 };

SDL_Rect source_texture_brique = { 0, 0, 32, 16 };

// -- Texture des enemies
SDL_Rect source_texture_chibre_bleu = {0,256,32,32};
SDL_Rect source_texture_pyramide_vert_verre = {0,288,32,32};
SDL_Rect source_texture_chromosome = {0,320,32,32};

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
SDL_Rect source_texture_bordure_porte_horizontale = {290, 128, 61, 16};

// -- Explosion
SDL_Rect src_explosion = {0, 384, 32, 32};


int topMargin = 100;
int timer_porte = 0;
int reverse = 1;

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
    textures_2 = SDL_LoadBMP("./assets/Arkanoid_sprites.bmp");

    // Partie noire du sprite en transparence, sauf pour celui où l'on veut recuperer le noir
    SDL_SetColorKey(textures_ascii, true, 0);
    SDL_SetColorKey(textures_gameover, true, 0);
    SDL_SetColorKey(textures_objets, true, 0);
    SDL_SetColorKey(textures_2, true, 0);
}

void Dessine_Texture(SDL_Rect texture, int x, int y) {
    SDL_Rect curseur_texture = {x, y, 0, 0 };
    SDL_BlitSurface(textures_objets, &texture, surface_fenetre, &curseur_texture);
}

void Dessine_Texture_Bis(SDL_Rect texture, int x, int y) {
    SDL_Rect curseur_texture = {x, y, 0, 0 };
    SDL_BlitSurface(textures_2, &texture, surface_fenetre, &curseur_texture);
}

void Dessine_Noir(int x, int y) {
    SDL_Rect curseur_texture = {x, y, 0, 0 };
    SDL_BlitSurface(texture_noir, &source_texture_noir, surface_fenetre, &curseur_texture);
}

void Sprite_par_niveau(){
    switch(niveau_actuel) {
        case 1 :
            source_texture_fond.x = 0;
            source_texture_fond_sombre.x = 0;
            break;
        case 2 :
            source_texture_fond.x = 48;
            source_texture_fond_sombre.x = 48;
            break;
        case 3 :
            source_texture_fond.x = 96;
            source_texture_fond_sombre.x = 96;
            break;
        case 4 :
            source_texture_fond.x = 144;
            source_texture_fond_sombre.x = 144;
            break;
        case 5 :
            source_texture_fond.x = 192;
            source_texture_fond_sombre.x = 192;
            break;
        case 6 :
            source_texture_fond.x = 240;
            source_texture_fond_sombre.x = 240;
            break;
        default:
            source_texture_fond.x = 0;
            source_texture_fond_sombre.x = 0;
    }
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
    int counter = 0;
    for (int i = src_bordure_coin_gauche.w; i < surface_fenetre->w - src_bordure_coin_droit.w; i += src_bordure_horizontale.w) {
        if (counter == 7) {
            Dessine_Texture(source_texture_bordure_porte_horizontale, i, topMargin-1);
            i += source_texture_bordure_porte_horizontale.w - src_bordure_horizontale.w;
        } else if (counter == 22){
            Dessine_Texture(source_texture_bordure_porte_horizontale, i, topMargin-1);
            i += source_texture_bordure_porte_horizontale.w - src_bordure_horizontale.w;
        } else {
            Dessine_Texture(src_bordure_horizontale, i, topMargin);
        }
        counter++;
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

int Trouve_Index_Y_Brique(int code_couleur) {
    return code_couleur / 6;
}

void Dessine_Briques() {
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            if (briques[i][j].estBrique) {
                int valeur_index_y_brique = Trouve_Index_Y_Brique(briques[i][j].code_couleur);
                source_texture_brique.x = source_texture_brique.w * (briques[i][j].code_couleur % 6);
                source_texture_brique.y = source_texture_brique.h * valeur_index_y_brique;
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

void Dessine_Enemie_Chibre_Bleu(int x, int y) {
    Dessine_Texture(source_texture_chibre_bleu, x, y);
}

void Dessine_Enemie_Pyramide_Verte_Verre(int x, int y) {
    Dessine_Texture(source_texture_pyramide_vert_verre, x ,y);
}

void Dessine_Enemies(Enemy enemies[], int nombre_de_enemies) {
    for (int i = 0; i < nombre_de_enemies; i++) {
        if(enemies[i].estMort == false) {
            if(enemies[i].type == 'c') {
                Dessine_Enemie_Chromosome(enemies[i].pos_x, enemies[i].pos_y);
            } else if (enemies[i].type == 'b') {
                Dessine_Enemie_Chibre_Bleu(enemies[i].pos_x, enemies[i].pos_y);
            } else if (enemies[i].type == 'p') {
                Dessine_Enemie_Pyramide_Verte_Verre(enemies[i].pos_x, enemies[i].pos_y);
            }
        }
    }
}

void Dessine_Enemie_Chromosome(int x, int y) {
    Dessine_Texture(source_texture_chromosome, x, y);
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

void Afficher_Bonus_S() {
    SDL_Rect bonus = {stats_bonus.pos_x, stats_bonus.pos_y, source_texture_brique_bonus_s.w, source_texture_brique_bonus_s.h};
    int i = coord_x_brique_cassee;
    int j = coord_y_brique_cassee;
    Initialise_Bonus(briques[i][j].pos_x,briques[i][j].pos_y, 'S');
    SDL_BlitSurface(textures_objets, &source_texture_brique_bonus_s, surface_fenetre, &bonus);
}

void Animation_Porte_Haut() {
    Dessine_Texture(source_texture_bordure_porte_horizontale, porteUn.pos_x, topMargin-1);
    Dessine_Texture(source_texture_bordure_porte_horizontale, porteDeux.pos_x, topMargin-1);

    if (timer_porte % 5 == 0 && timer_porte < 50) {
        source_texture_bordure_porte_horizontale.y += (source_texture_bordure_porte_horizontale.h * reverse);
        if (source_texture_bordure_porte_horizontale.y == 208 || source_texture_bordure_porte_horizontale.y == 128) {
            reverse *= -1;
        }
    }

    if (timer_porte < 50) {
        timer_porte++;
    }
}

void Animation_Enemies() {
    for(int i = 0; i < 3; i++) {
        if (enemies[i].type == 'c') {
            Animation_Enemie_Chromosome(i);
        } else if (enemies[i].type == 'b') {
            Animation_Enemie_Chibre_Bleu(i);
        } else if (enemies[i].type == 'p') {
            Animation_Enemie_Pyramide_Vert_Verre(i);
        }
    }
}

void Animation() {
    // Animation du bonus
    Met_A_Jour_Position_Bonus();

    // Animation brique solide
    Animation_Brique_Solide();

    // Animation ouverture potes
    Animation_Porte_Haut();

    // Animation des enemies
    Animation_Enemies();

    // Explosion enemies
    Animation_Explosion();
}