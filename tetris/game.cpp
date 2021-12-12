#include "game.h"


// Graphics library
#include <SDL.h>

//STL
#include <string>
#include <memory>
#include <iostream>
#include <random>

//System
#include <cmath>

// Project
#include "shapes.h"
#include "piece.h"
#include "board.h"


Game::Game()
{
	quit=false;
	pause=false;
	last_piece_fallen_p1=true;
	manual_fall_p1=false;
	max_speed=0.15;
	manual_fall_p2=false;
	last_piece_fallen_p2=true;
	board_p1 = nullptr;
	board_p2 = nullptr;
}

void Game::initialize(int nb_player)
{
	quit=false;
	pause=false;
	last_piece_fallen_p1=true;
	manual_fall_p1=false;
	max_speed=0.15;
	manual_fall_p2=false;
	last_piece_fallen_p2=true;
	board_p1 = nullptr;
	board_p2 = nullptr;

	board_p1= new Board();
	board_p1->setGhostPiece(board_p1->createRandomPiece());
	board_p1->ghostToCurrent();

	if(nb_player==2)
	{
		board_p2= new Board();
		board_p2->setGhostPiece(board_p2->createRandomPiece());
		board_p2->ghostToCurrent();
	}
	
}


void Game::finalize(int nb_player)
{
	board_p1->clear();
	delete board_p1;
	if(nb_player==2)
	{
		board_p2->clear();
		delete board_p2;
	}

}

void Game::keyboard(const Uint8* keys, int nb_joueur,int gameover_p1, int gameover_p2)
{
	if (keys[SDL_SCANCODE_SPACE])
	{
		pause=!pause;
	}
	if(!pause)
	{
		if (nb_joueur==1 && !gameover_p1)  //TOUCHE POUR JEU SOLO
		{
			if (keys[SDL_SCANCODE_DOWN])
			{
				board_p1->dropCurrentPiece();
				manual_fall_p1=true;
			}
			if (keys[SDL_SCANCODE_LEFT])
			{
				board_p1->moveCurrentPieceLeft();
			}
			if (keys[SDL_SCANCODE_RIGHT] )
			{
				board_p1->moveCurrentPieceRight();
			}
			if (keys[SDL_SCANCODE_E] )
			{
				board_p1->rotateCurrentPieceRight();
			}
			if (keys[SDL_SCANCODE_Q] )
			{
				board_p1->rotateCurrentPieceLeft();
			}
		}
		else //TOUCHE POUR JEU DUO
		{
			// J1
			if (!gameover_p1)
			{
				if (keys[SDL_SCANCODE_E] )
				{
					board_p1->rotateCurrentPieceRight();
				}
				if (keys[SDL_SCANCODE_Q] )
				{
					board_p1->rotateCurrentPieceLeft();
				}
				if (keys[SDL_SCANCODE_S])
				{
					board_p1->dropCurrentPiece();
					manual_fall_p1=true;
				}
				if (keys[SDL_SCANCODE_A])
				{
					board_p1->moveCurrentPieceLeft();
				}
				if (keys[SDL_SCANCODE_D] )
				{
					board_p1->moveCurrentPieceRight();
				}
			}
			

			//J2
			if (!gameover_p2)
			{
				if (keys[SDL_SCANCODE_DOWN])
				{
					board_p2->dropCurrentPiece();
					manual_fall_p2=true;
				}
				if (keys[SDL_SCANCODE_LEFT])
				{
					board_p2->moveCurrentPieceLeft();
				}
				if (keys[SDL_SCANCODE_RIGHT] )
				{
					board_p2->moveCurrentPieceRight();
				}
				if (keys[SDL_SCANCODE_RCTRL] )
				{
					board_p2->rotateCurrentPieceLeft();
				}
				if (keys[SDL_SCANCODE_KP_0] )
				{
					board_p2->rotateCurrentPieceRight();
				}
			}
			
		}
	}
}



bool Game::loop(Window* fenetre)
{
	Uint64 now = SDL_GetPerformanceCounter(); // timer
	Uint64 prev = now;
	int n_deleted_line =0;
	double delta_t=0;
	float speed=0.5;
	int score = board_p1->getScore();
	double total_time=0;

	Mix_Chunk *line;
	Mix_Chunk *tetris;
	Mix_AllocateChannels(5);
	line = Mix_LoadWAV("audio/line.wav");
	tetris = Mix_LoadWAV("audio/clear.wav");
	Mix_VolumeChunk(line, MIX_MAX_VOLUME/4);
	Mix_VolumeChunk(tetris, MIX_MAX_VOLUME/4);
	

	while ( !quit )
	{
		// Event management
		SDL_Event event;
		while ( !quit && SDL_PollEvent( &event ) )
		{
			switch ( event.type )
			{
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_WINDOWEVENT:
					switch (event.window.event) {

					case SDL_WINDOWEVENT_CLOSE:   // exit game
						Mix_FreeChunk(line);
						Mix_FreeChunk(tetris);
						return true;
						break;

					default:
						break;
					}
					break;

				// Keyboard management
				case SDL_KEYDOWN:
					const Uint8* state = SDL_GetKeyboardState(NULL);
					keyboard( state , 1 , 0, 0);
					quit |= state[ SDL_SCANCODE_ESCAPE ];
					fenetre->update(board_p1->area, board_p1->getGhostPiece(),score);
					break;
			}
		}
		
		// Gestion du Jeu
		if(!pause)
		{
			prev = now;
			now = SDL_GetPerformanceCounter();
			delta_t += (double)((now - prev) / (double)SDL_GetPerformanceFrequency());
			total_time += (double)((now - prev) / (double)SDL_GetPerformanceFrequency());

			// Si le joueur vient de faire tomber une pièce, 
			// ou si une pièce est tombée et que le tick de temps est passé: 
			if((manual_fall_p1)||(board_p1->isCurrentPieceFallen() && (delta_t > std::max(speed-(board_p1->getScore()/100000.0),max_speed))))
			{
				// On efface les lignes a effacer
				n_deleted_line = board_p1->deletePossibleLines();
				// on met a jour le score 
				switch (n_deleted_line)
				{
					case 0:
						/* Rien */
						break;
					
					case 1:
						score+=500;
						Mix_PlayChannel(-1, line, 0);
						break;

					case 2:
						score+=2000;
						Mix_PlayChannel(-1, line, 0);
						break;

					case 3:
						score+=5000;
						Mix_PlayChannel(-1, line, 0);
						break;
					
					case 4:
						score+=10000;
						Mix_PlayChannel(-1, tetris, 0);
						break;
					
					default:
						break;
				}
				board_p1->setScore(score);
				
				// On check si la prochaine pièce est spawnable
				if(!board_p1->isPieceSwpanable(board_p1->getGhostPiece()))
				{	
					// Si non, alors Game Over
					quit=true;
				}
				else
				{ 
					// Si oui, alors on transfert la prochaine pièce sur le plateau
					board_p1->ghostToCurrent();
					// Marqueur pour dire que le prochain tick
					// doit faire apparaitre et pas descendre la nouvelle pièce
					last_piece_fallen_p1=true;
					fenetre->update(board_p1->area, board_p1->getGhostPiece(),score);
				}
			}

			// Si un Tick de temps est passé mais que la pièce courante est pas au sol:
			if(delta_t > std::max(speed-(board_p1->getScore()/100000.0),max_speed)&& !last_piece_fallen_p1)
			{
				// La pièce descend d'un cran
				board_p1->moveCurrentPieceDown();
				delta_t=0;
				fenetre->update(board_p1->area, board_p1->getGhostPiece(),score);
			}

			// Si une pièce a été drop manuellement ou
			// que la dernière pièce a été posée et que le tick de temps est passé
			if((manual_fall_p1)||(delta_t > std::max(speed-(board_p1->getScore()/100000.0),max_speed)&& last_piece_fallen_p1))
			{
				//on ne fait pas tomber la pièce mais on l'affiche juste  
				last_piece_fallen_p1=false;								
				delta_t=0;												
				fenetre->update(board_p1->area, board_p1->getGhostPiece(),score);
			}
			manual_fall_p1=false;
		}
		//std::cout << "\r\e[K total_time:" << total_time <<std::flush;
	}	

	Mix_FreeChunk(line);
	Mix_FreeChunk(tetris);

	return false;

}

// Ici c'est comme pour la loop 1 joueur mais tout est dédoublé pour en avoir 2
// les seules différences sont les condition d'arret de la game et l'ajout de ligne a l'autre
bool Game::loop_duo(Window* fenetre)
{
	Uint64 now = SDL_GetPerformanceCounter(); // timers
	Uint64 prev = now;
	int n_deleted_line_p1 =0;
	int n_deleted_line_p2 =0;
	double delta_t_p1=0;
	double delta_t_p2=0;
	float speed_p1=0.5;
	float speed_p2=0.5;
	int score_p1 = board_p1->getScore();
	int score_p2 = board_p2->getScore();

	Mix_Chunk *line;
	Mix_Chunk *tetris;
	Mix_AllocateChannels(5);
	line = Mix_LoadWAV("audio/line.wav");
	tetris = Mix_LoadWAV("audio/clear.wav");
	Mix_VolumeChunk(line, MIX_MAX_VOLUME/4);
	Mix_VolumeChunk(tetris, MIX_MAX_VOLUME/4);

	int gameover_p1=0;
	int gameover_p2=0;

	double total_time=0;

	while ( !quit )
	{
		// Event management
		SDL_Event event;
		while ( !quit && SDL_PollEvent( &event ) )
		{
			switch ( event.type )
			{
				case SDL_QUIT:
					quit = true;
					break;

				// Keyboard management
				case SDL_KEYUP:
					//const Uint8* keyup = SDL_GetKeyboardState(NULL);
					
					break;

				case SDL_WINDOWEVENT:
					switch (event.window.event) {

					case SDL_WINDOWEVENT_CLOSE:   // exit game
						Mix_FreeChunk(line);
						Mix_FreeChunk(tetris);
						return true;
						break;

					default:
						break;
					}
					break;

				case SDL_KEYDOWN:
					const Uint8* keypressed= SDL_GetKeyboardState(NULL);
					keyboard( keypressed , 2 ,gameover_p1,gameover_p2);
					quit |= keypressed[ SDL_SCANCODE_ESCAPE ];
					fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
					break;
				
			}
		}


		if(!pause)
		{
			prev = now;
			now = SDL_GetPerformanceCounter();
			delta_t_p1 += (double)((now - prev) / (double)SDL_GetPerformanceFrequency());
			delta_t_p2 += (double)((now - prev) / (double)SDL_GetPerformanceFrequency());
			
			total_time += (double)((now - prev) / (double)SDL_GetPerformanceFrequency());

			// check P1
			if (!gameover_p1)
			{
				if((manual_fall_p1)||(board_p1->isCurrentPieceFallen() && (delta_t_p1 > std::max(speed_p1-(board_p1->getScore()/100000.0),max_speed))))
				{
					n_deleted_line_p1 = board_p1->deletePossibleLines();
					switch (n_deleted_line_p1)
					{
						case 0:
							/* Rien */
							break;
						
						case 1:
							score_p1+=500;
							if(!gameover_p2)
								board_p2->add_lines(1);
							Mix_PlayChannel(-1, line, 0);
							break;

						case 2:
							score_p1+=2000;
							if(!gameover_p2)
							{
								board_p2->add_lines(1);
							}
							Mix_PlayChannel(-1, line, 0);
							break;

						case 3:
							score_p1+=5000;
							if(!gameover_p2)
							{
								board_p2->add_lines(1);
							}
							Mix_PlayChannel(-1, line, 0);
							break;
						
						case 4:
							score_p1+=10000;
							if(!gameover_p2)
							{
								board_p2->add_lines(2);
							}
							Mix_PlayChannel(-1, tetris, 0);
							break;
						
						default:
							break;
					}
					board_p1->setScore(score_p1);
					
					if(board_p1->isPieceSwpanable(board_p1->getGhostPiece()))
					{
						board_p1->ghostToCurrent();
						last_piece_fallen_p1=true;
						fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
					}
					else 
					{
						gameover_p1=1;
						if( board_p2->getScore() > board_p1->getScore())
						{
							quit=true;
						}
						
					}
					
				}

				if(delta_t_p1 > std::max(speed_p1-(board_p1->getScore()/100000.0),max_speed)&& !last_piece_fallen_p1)
				{
					board_p1->moveCurrentPieceDown();
					delta_t_p1=0;
					fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
				}
				if((manual_fall_p1)||(delta_t_p1 > std::max(speed_p1-(board_p1->getScore()/100000.0),max_speed)&& last_piece_fallen_p1))
				{
					last_piece_fallen_p1=false;								
					delta_t_p1=0;												
					fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
				}
				manual_fall_p1=false;
			}


			// check P2
			if(!gameover_p2)
			{
				if((manual_fall_p2)||(board_p2->isCurrentPieceFallen() && (delta_t_p2 > std::max(speed_p2-(board_p2->getScore()/100000.0),max_speed))))
				{
					n_deleted_line_p2 = board_p2->deletePossibleLines();
					switch (n_deleted_line_p2)
					{
						case 0:
							/* Rien */
							break;
						
						case 1:
							score_p2+=500;
							if(!gameover_p1)
								board_p1->add_lines(1);
							Mix_PlayChannel(-1, line, 0);
							break;

						case 2:
							score_p2+=2000;
							if(!gameover_p1)
								board_p1->add_lines(1);
							Mix_PlayChannel(-1, line, 0);
							break;

						case 3:
							score_p2+=5000;
							if(!gameover_p1)
								board_p1->add_lines(1);
							Mix_PlayChannel(-1, line, 0);
							break;
						
						case 4:
							score_p2+=10000;
							if(!gameover_p1)
								board_p1->add_lines(2);
							Mix_PlayChannel(-1, tetris, 0);
							break;
						
						default:
							break;
					}
					board_p2->setScore(score_p2);
					
					if(board_p2->isPieceSwpanable(board_p2->getGhostPiece()))
					{
						board_p2->ghostToCurrent();
						last_piece_fallen_p2=true;
						fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
					}
					else
					{
						gameover_p2=1;
						
					}
				}

				if(delta_t_p2 > std::max(speed_p2-(board_p2->getScore()/100000.0),max_speed)&& !last_piece_fallen_p2)
				{
					board_p2->moveCurrentPieceDown();
					delta_t_p2=0;
					fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
				}
				if((manual_fall_p2)||(delta_t_p2 > std::max(speed_p2-(board_p2->getScore()/100000.0),max_speed)&& last_piece_fallen_p2))
				{
					last_piece_fallen_p2=false;								
					delta_t_p2=0;												
					fenetre->update_duo(board_p1->area,board_p2->area, board_p1->getGhostPiece(), board_p2->getGhostPiece(),score_p1, score_p2);
				}

				manual_fall_p2=false;
			}

			// Cas d'arrets de la partie :
			if( gameover_p2 && (board_p2->getScore() < board_p1->getScore()))
			{
				//Victoire par score de P1
				quit=true;
			}
			if( gameover_p1 && (board_p2->getScore() > board_p1->getScore()))
			{
				//Victoire par score de P2
				quit=true;
			}
			if (gameover_p1 && gameover_p2)
			{
				// Double game over
				quit=true;
			}
			
		}
		//std::cout << "\r\e[K total_time:" << total_time <<std::flush;
	}

	Mix_FreeChunk(line);
	Mix_FreeChunk(tetris);

	return false;
}