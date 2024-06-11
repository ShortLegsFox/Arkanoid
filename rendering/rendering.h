#ifndef RENDERING_H
#define RENDERING_H

#include <SDL.h>
#include "../game-objects/enemy.h"

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
extern SDL_Rect source_texture_chibre_bleu;
extern SDL_Rect source_texture_pyramide_vert_verre;
extern SDL_Rect source_texture_chromosome;
extern SDL_Rect src_explosion;

// -- Game over
extern SDL_Rect source_texture_gameover;

// -- Bonus
extern SDL_Rect source_texture_brique_bonus_s;
extern SDL_Rect source_texture_brique_bonus_l;

extern int topMargin;
extern int timer_porte;

void Initialise_Fenetre();
void Initialise_Sprites();

void Dessine_Fond();
void Dessine_Bordure();
void Dessine_Briques();
void Dessine_Vaisseau(int x, int y);
void Dessine_Balle(int x, int y);
void Dessine_Enemie_Chibre_Bleu(int x, int y);
void Dessine_Enemie_Pyramide_Verte_Verre(int x, int y);
void Dessine_Enemie_Chromosome(int x, int y);
void Dessine_Enemies(Enemy enemies[], int nombre_de_enemies);
void Dessine_Texture(SDL_Rect texture, int x, int y);
void Dessine_Texture_Bis(SDL_Rect texture, int x, int y);
void Sprite_par_niveau();

void CalculRectangleCaractereSprite(char character, SDL_Rect* sourceRect, int spriteWidth, int spriteHeight, int charsPerLine);
void AfficheRectangleCaractereSprite(char character, int coord_x, int coord_y);
void AfficheRectangleTextSprite(char text[], int coord_x, int coord_y);
void Afficher_Game_Over();
void Afficher_Bonus_S();
void Afficher_Bonus_L();
void Initialise_Position_Bonus(SDL_Rect sourceTextureBonus, char typeBonus);
void Animation();

#endif