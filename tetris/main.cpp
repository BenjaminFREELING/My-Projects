
// Graphics library
#include <SDL.h>
#include <ctime>

// Project
#include "game.h"


/******************************************************************************
 * Main entry program
 *
 * @param argc Number of arguments
 * @param argv List of arguments
 *
 * @return flag telling whether or not it succeeds
 ******************************************************************************/
int main( int argc, char** argv )
{

	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	// Initialize graphics library
	if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) != 0 )
	{
		return 1;
	}

	// Audio management

	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
	Mix_Music* gameMusic = Mix_LoadMUS("./audio/tetris.mp3");
	Mix_PlayMusic(gameMusic,-1);
	Mix_VolumeMusic(MIX_MAX_VOLUME/4);

	int game_state = 0;
	bool forcequit = false;
	while(game_state!=2 && !forcequit)
	{

		//Menu
		Window* menu= new Window("Tetris Menu",1000,800, 25);
		menu->initialize();
		bool quit_menu=false;
		
		menu->draw_menu(game_state);
		while(!quit_menu)
		{
			// Event management
			SDL_Event event;
			while ( !quit_menu && SDL_PollEvent( &event ) )
			{
				switch ( event.type )
				{
					case SDL_QUIT:
						quit_menu=true;
						forcequit=true;
						game_state=2;
						break;

					case SDL_WINDOWEVENT:
						switch (event.window.event) {

						case SDL_WINDOWEVENT_CLOSE:   // exit game
							quit_menu=true;
							forcequit=true;
							game_state=2;
							break;

						default:
							break;
						}
						break;

					// Keyboard management
					case SDL_KEYDOWN :

						const Uint8* state = SDL_GetKeyboardState(NULL);
						if(state[ SDL_SCANCODE_ESCAPE ])
						{
							game_state=2;
							quit_menu=true;
						}
						if (state[ SDL_SCANCODE_DOWN ])		
						{
							if(game_state==2)
								game_state=0;
							else
								game_state++;
						}
						if (state[ SDL_SCANCODE_UP ])
						{
							if(game_state==0)
								game_state=2;
							else
								game_state--;
						}
						if (state[ SDL_SCANCODE_RETURN ])
						{
							quit_menu=true;
						}

					menu->draw_menu(game_state);
				}
			}
		}
		menu->finalize();

		Window* fenetre;
		Game* game;
		int restart=0;

		// JEU
		switch (game_state)
		{
			case 0: // JEU SOLO
				fenetre= new Window("Tetris",1000,800, 25);
				fenetre->initialize();


				while (!restart && !forcequit)
				{
					// Game management
					game = new Game();
					// - initialization
					game->initialize(1);
					// - main event loop
					forcequit = game->loop(fenetre);

					if (!forcequit)
					{
						// choix de l'action en fin de partie
						restart = fenetre->endScreen(game,1);
					}
					
					
					// - finalization
					game->finalize(1);
					delete game;
					game = nullptr;
				}

				
				// Finalize graphics library
				fenetre->finalize();
				break;
			
			case 1 : //JEU DUO 1V1 MAGGLE
				fenetre= new Window("Tetris",1100,800, 25);
				fenetre->initialize();

				while (!restart && !forcequit)
				{
					// Game management
					game = new Game();
					// - initialization
					game->initialize(2);
					// - main event loop
					forcequit = game->loop_duo(fenetre);

					if (!forcequit)
					{
						restart = fenetre->endScreen(game,2);
					}

					// - finalization
					game->finalize(2);
					delete game;
					game = nullptr;

				}
			
				// Finalize graphics library
				fenetre->finalize();
				break;
			
			case 2 : // QUITTER
				break;
		}
	}

	//Finalize Music
	Mix_FreeMusic(gameMusic);
	Mix_CloseAudio();

	SDL_Quit();

	return 0;
}