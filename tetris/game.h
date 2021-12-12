#ifndef _GAME_H_
#define _GAME_H_

// STL
#include <cstdint>
#include <vector>

// Project
#include "shapes.h"
#include "piece.h"
#include "board.h"
#include "window.h"

// Project
class Piece;
class Board;
class Window;


/**
  * @class Game
  * @brief The Game class provides interface to the game management.
  */
class Game
{

public:

	/** @brief plateaux de jeux des joueurs
	 */ 
	Board* board_p1;
	Board* board_p2;
    
	Game();

	/** @brief Initialisation de la classe pour son utilisation
	 * */
	void initialize(int nb_player);
	
	/** @brief Finalisation de la classe
	 * */
	void finalize(int nb_player);
	
	
	/** @brief Boucle de jeu en mode solo
	 * */
	bool loop(Window* fenetre);

	
	/** @brief Boucle de jeu en mode duo
	 * */
	bool loop_duo(Window* fenetre);


protected:
	/** @brief Gestion du clavier*/
	void keyboard( const std::uint8_t* keys, int nb_joueur, int gameover_p1, int gameover_p2 );


private:

	double max_speed;
	bool quit;
	bool pause;
	bool manual_fall_p1;
	bool last_piece_fallen_p1;

	bool manual_fall_p2;
	bool last_piece_fallen_p2;

};

#endif
