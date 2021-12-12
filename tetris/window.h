#ifndef _WINDOW_
#define _WINDOW_


// STL
#include <string>
#include <SDL.h>
#include <vector>
#include "game.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// Graphics library
struct SDL_Window;
struct SDL_Surface;
struct SDL_Color;
struct SDL_Texture;
struct SDL_Rect;

// Project
class Game;


/**
  * @class Window
  *
  * @brief The Window class provides interface to the underlying graphics library's window management.
  */
class Window
{


public:

	Window();
	Window( const char* name, const int w, const int h, const int pixel_size );

	inline int width() const;
	inline int height() const;

	/** @brief  Initialisation de la fenetre*/
	void initialize();
	/** @brief  Finalisation de la Fenetre*/
	void finalize();

	/** @brief Dessine le background du menu */
	void draw_background();
	/** @brief dessine le background du mode Solo */
	void draw_background_playzone();
	/** @brief  Dessine le background du mode duo*/
	void draw_background_playzone_duo();

	/** @brief  Dessine la grille du plateau*/
	void draw_grid();
	/** @brief  Dessine les grilles des 2 plateaux en mode duo*/
	void draw_grid_duo();

	/** @brief  Dessine les pièces à l'écran selon la matrice*/
	void draw_gamestate(int area [20][10] );
	/** @brief  Dessine les pièces des 2 joueurs à l'écran selon leurs matrices*/
	void draw_gamestate_duo(int area_p1 [20][10],int area_p2 [20][10] );

	/** @brief  Dessine la Ghost piece dans sa zone*/
	void draw_ghost_piece(Piece ghostPiece);
	/** @brief  Dessine les 2 ghostPiece des joueurs en mode duo*/
	void draw_ghost_piece_duo(Piece ghostPiece_p1,Piece ghostPiece_p2);

	/** @brief  affiche le score au bon endroit en mode solo*/
	void draw_score(int score);
	/** @brief  Affiche les Scores des joueurs aux bons endroit en mode duo*/
	void draw_score_duo(int score_p1,int score_p2);

	/** @brief  Met l'affichage à jour d'une partie solo */
	void update(int area [20][10],Piece ghostPiece,int score);
	/** @brief  Met l'affichage a jour d'une partie duo*/
	void update_duo(int area_p1 [20][10],int area_p2 [20][10], Piece ghostPiece_p1,Piece ghostPiece_p2, int score_p1, int score_p2);

	/** @brief  Dessine le menu*/
	void draw_menu(int game_state);

	/** @brief  Dessine l'écran de fin de la partie*/
	int endScreen(Game* g,int nb_player);
	
	/** @brief  Dessine l'écran de fin d'un partie SOLO*/
	void draw_endScreen(int game_state,int area [20][10], int score);
	/** @brief  Dessine l'écran de fin d'une partie DUO*/
	void draw_endScreen_2p(int game_state, Game *g);


protected:

	/**
	 * Graphics library
	 */
	SDL_Window* pWindow_;
	SDL_Renderer* pRenderer_;
	SDL_Surface* pSurface_;
	SDL_Texture* pTexture_;

	SDL_Surface* surfaceText_p1;
	SDL_Texture* textureText_p1;

	SDL_Surface* surfaceText_p2;
	SDL_Texture* textureText_p2;	
	TTF_Font* font;
	

	/**
	 * Size 
	 */
	int w_;
	int h_;
	int pixel_size_;
	/**
	 * Name
	 */
	const char* title_;

	

private:

	

};

#include "window.inl"

#endif
