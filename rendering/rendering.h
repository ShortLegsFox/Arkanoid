#ifndef RENDERING_H
#define RENDERING_H

#include <SDL2/SDL.h>

extern SDL_Window* pointeur_fenetre; // Pointeur vers la fenetre SDL
extern SDL_Surface* surface_fenetre; // Surface de la fenetre
extern SDL_Surface* texture_noir; // Planche de la texture de la fenetre
extern SDL_Surface* textures_objets; // Planche des textures des objets (briques)
extern SDL_Surface* textures_ascii; // Planche des textures des ASCII (aplphabet et chiffres)
extern SDL_Surface* textures_gameover; // Planche des textures game over

// -- Découpage sur la planche de texture --
extern SDL_Rect source_texture_fond; // Le point (0,0) est en haut a gauche
extern SDL_Rect source_texture_fond_sombre;
extern SDL_Rect source_texture_balle;
extern SDL_Rect source_texture_vaisseau;
extern SDL_Rect source_texture_brique;
extern SDL_Rect src_bordure_verticale;
extern SDL_Rect src_bordure_horizontale;
extern SDL_Rect src_bordure_coin_gauche;
extern SDL_Rect src_bordure_coin_droit;
extern SDL_Rect source_texture_bordure_porte_verticale;
extern SDL_Rect source_texture_bordure_porte_horizontale;

// -- Game over
extern SDL_Rect source_texture_gameover;

// -- Bonus
extern SDL_Rect source_texture_brique_bonus_s;

extern int topMargin;

void Initialise_Fenetre();
void Initialise_Sprites();

void Dessine_Fond();
void Dessine_Bordure();
void Dessine_Briques();
void Dessine_Vaisseau(int x, int y);
void Dessine_Balle(int x, int y);

void CalculRectangleCaractereSprite(char character, SDL_Rect* sourceRect, int spriteWidth, int spriteHeight, int charsPerLine);
void AfficheRectangleCaractereSprite(char character, int coord_x, int coord_y);
void AfficheRectangleTextSprite(char text[], int coord_x, int coord_y);
void Afficher_Game_Over();
void Afficher_Bonus_S();
void Animation();

#endif