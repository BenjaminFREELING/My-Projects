#include "window.h"

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// Graphics library



/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

/******************************************************************************
 * Default constructor
 ******************************************************************************/
Window::Window()
:	pWindow_( nullptr )
,	pRenderer_( nullptr )
,	pSurface_( nullptr )
,	pTexture_( nullptr )
,	surfaceText_p1( nullptr )
,	textureText_p1( nullptr )
,	surfaceText_p2( nullptr )
,	textureText_p2( nullptr )
,	font( nullptr )
,	w_( 600 )
,	h_( 800 )
,	pixel_size_( 30 )
,	title_( "Fenêtre" )
{
}

/******************************************************************************
 * Constructor
 ******************************************************************************/
Window::Window( const char* name, int w, int h, int pixel_size )
:	pWindow_( nullptr )
,	pRenderer_( nullptr )
,	pSurface_( nullptr )
,	pTexture_( nullptr )
,	surfaceText_p1( nullptr )
,	textureText_p1( nullptr )
,	surfaceText_p2( nullptr )
,	textureText_p2( nullptr )
,	font( nullptr )
,	w_( w )
,	h_( h )
,	pixel_size_ ( pixel_size )
,	title_( name )
{

}

/******************************************************************************
 * Initialize
 ******************************************************************************/
void Window::initialize()
{
	SDL_CreateWindowAndRenderer( w_, h_, SDL_WINDOW_SHOWN, &pWindow_, &pRenderer_);
	SDL_SetWindowTitle(pWindow_, title_);
	pSurface_=SDL_LoadBMP("./images/tetris_wallpaper.bmp");
	pTexture_=SDL_CreateTextureFromSurface(pRenderer_, pSurface_);
	TTF_Init();
	font = TTF_OpenFont("font/block.ttf", 30);

}

/******************************************************************************
 * Finalize
 ******************************************************************************/
void Window::finalize()
{
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_DestroyTexture(pTexture_);
	SDL_FreeSurface(pSurface_);
	SDL_DestroyRenderer(pRenderer_);
	SDL_DestroyWindow(pWindow_);
}

/******************************************************************************
 * draw_background
 ******************************************************************************/
void Window::draw_background()
{
	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 255); 
	SDL_RenderClear(pRenderer_);  

	SDL_RenderCopy(pRenderer_, pTexture_, nullptr, nullptr); // Affiche la texture sur tout l'écran
}

void Window::draw_background_playzone()
{

	int x_playzone = (w_/2)-(pixel_size_*5);
	int y_playzone = (h_ /2)-(pixel_size_*10);
	SDL_Rect playzone { x_playzone, y_playzone, pixel_size_*10, pixel_size_*20 };

	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 255);
	SDL_RenderFillRect(pRenderer_, &playzone);

	SDL_Rect ghostPieceZone {x_playzone+pixel_size_*11,y_playzone,pixel_size_*6,pixel_size_*6};
	SDL_RenderFillRect(pRenderer_, &ghostPieceZone);

	SDL_Rect scoreZone {x_playzone+pixel_size_*11,y_playzone+7*pixel_size_,pixel_size_*6,pixel_size_*2};
	SDL_RenderFillRect(pRenderer_, &scoreZone);

	SDL_Rect keyZone {2*pixel_size_,h_-2*pixel_size_,w_-4*pixel_size_,pixel_size_*1};
	
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 100);

	SDL_RenderFillRect(pRenderer_, &keyZone);
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_NONE);

        surfaceText_p1 = TTF_RenderText_Solid(font,"Keys: move-right:->  move-left:<-  drop:d-arrow  rotate:a/e  pause:space  FF:escape", {255,255,255});

	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);

	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &keyZone);


	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);

	SDL_RenderDrawRect(pRenderer_, &ghostPieceZone);
	SDL_RenderDrawRect(pRenderer_, &scoreZone);
	SDL_RenderDrawRect(pRenderer_, &keyZone);


}

void Window::draw_background_playzone_duo()
{

	int x_playzone_p1 = (w_/5)-(pixel_size_*5);
	int x_playzone_p2 = ((4*w_)/6)-(pixel_size_*4);
	int y_playzone = (h_ /2)-(pixel_size_*10);
	SDL_Rect playzone_p1 { x_playzone_p1, y_playzone, pixel_size_*10, pixel_size_*20 };
	SDL_Rect playzone_p2 { x_playzone_p2, y_playzone, pixel_size_*10, pixel_size_*20 };

	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 255);
	SDL_RenderFillRect(pRenderer_, &playzone_p1);
	SDL_RenderFillRect(pRenderer_, &playzone_p2);

	SDL_Rect ghostPieceZone_p1 {x_playzone_p1+pixel_size_*11,y_playzone,pixel_size_*6,pixel_size_*6};
	SDL_Rect ghostPieceZone_p2 {x_playzone_p2+pixel_size_*11,y_playzone,pixel_size_*6,pixel_size_*6};

	SDL_RenderFillRect(pRenderer_, &ghostPieceZone_p1);
	SDL_RenderFillRect(pRenderer_, &ghostPieceZone_p2);

	SDL_Rect scoreZone_p1 {x_playzone_p1+pixel_size_*11,y_playzone+7*pixel_size_,pixel_size_*6,pixel_size_*2};
	SDL_Rect scoreZone_p2 {x_playzone_p2+pixel_size_*11,y_playzone+7*pixel_size_,pixel_size_*6,pixel_size_*2};

	SDL_RenderFillRect(pRenderer_, &scoreZone_p1);
	SDL_RenderFillRect(pRenderer_, &scoreZone_p2);

	SDL_Rect keyZone {4*pixel_size_,h_-3*pixel_size_,w_-8*pixel_size_,pixel_size_*2};
	
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 100);

	SDL_RenderFillRect(pRenderer_, &keyZone);
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_NONE);
	SDL_Rect keyZone_p1 {4*pixel_size_,h_-3*pixel_size_,w_-8*pixel_size_,pixel_size_*1};
        surfaceText_p1 = TTF_RenderText_Solid(font,"Keys P1: move right:q  move left:d  drop:s  rotate: a/e" , {255,255,255});

	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);

	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &keyZone_p1);

	SDL_Rect keyZone_p2 {4*pixel_size_,h_-2*pixel_size_,w_-8*pixel_size_,pixel_size_*1};
        surfaceText_p1 = TTF_RenderText_Solid(font,"Keys P2: move right:->  move left:<-  drop:d-arrow  rotate:l-ctrl/keypad-0" , {255,255,255});

	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);

	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &keyZone_p2);
 

	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);

	SDL_RenderDrawRect(pRenderer_, &ghostPieceZone_p1);
	SDL_RenderDrawRect(pRenderer_, &scoreZone_p1);
	SDL_RenderDrawRect(pRenderer_, &ghostPieceZone_p2);
	SDL_RenderDrawRect(pRenderer_, &scoreZone_p2);
}

void Window::draw_grid()
{
	//playzone grid
	int x_playzone = (w_/2)-(pixel_size_*5);
	int y_playzone = (h_ /2)-(pixel_size_*10);
	SDL_Rect playzone { x_playzone, y_playzone, pixel_size_*10, pixel_size_*20 };

	SDL_Rect playzone_rects [200];

	int n=0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			SDL_Rect grid { x_playzone+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects[n] = grid;
			n++;
		}
	}
	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);
	SDL_RenderDrawRects(pRenderer_, playzone_rects, 200);
}

void Window::draw_grid_duo()
{
	//playzone grid
	int x_playzone_p1 = (w_/5)-(pixel_size_*5);
	int x_playzone_p2 = ((4*w_)/6)-(pixel_size_*4);
	int y_playzone = (h_ /2)-(pixel_size_*10);
	SDL_Rect playzone_p1 { x_playzone_p1, y_playzone, pixel_size_*10, pixel_size_*20 };
	SDL_Rect playzone_p2 { x_playzone_p2, y_playzone, pixel_size_*10, pixel_size_*20 };

	SDL_Rect playzone_rects_p1 [200];
	SDL_Rect playzone_rects_p2 [200];

	int n_p1=0;
	int n_p2=0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			SDL_Rect grid_p1 { x_playzone_p1+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects_p1[n_p1] = grid_p1;
			n_p1++;
			SDL_Rect grid_p2 { x_playzone_p2+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects_p2[n_p2] = grid_p2;
			n_p2++;
		}
	}
	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);
	SDL_RenderDrawRects(pRenderer_, playzone_rects_p1, 200);
	SDL_RenderDrawRects(pRenderer_, playzone_rects_p2, 200);

}

void Window::draw_gamestate(int area[20][10] )
{
	int x_playzone = (w_/2)-(pixel_size_*5);
	int y_playzone = (h_ /2)-(pixel_size_*10);
	
	SDL_Rect playzone_rects [200];
	SDL_Rect playzone_rects_color [7][200];

	int n=0;
	int n_color [7]={0};
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int val_case=area[i][j];
			if (val_case!=0)
			{
				SDL_Rect grid { x_playzone+j*pixel_size_, y_playzone+i*pixel_size_, pixel_size_, pixel_size_ };

				playzone_rects_color[val_case-1][n_color[val_case-1]] = grid;

				n_color[val_case-1]++;
			}	
		}
	}

	// Couleurs des pièces (ptet une boucle for c'est mieux mais c'est moins pratique)
	SDL_SetRenderDrawColor(pRenderer_, 255, 255, 10, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[0], n_color[0]);

	SDL_SetRenderDrawColor(pRenderer_, 0, 255, 255, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[1], n_color[1]);

	SDL_SetRenderDrawColor(pRenderer_, 0, 255, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[2], n_color[2]);

	SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[3], n_color[3]);

	SDL_SetRenderDrawColor(pRenderer_, 0, 50, 255, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[4], n_color[4]);

	SDL_SetRenderDrawColor(pRenderer_, 255, 140, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[5], n_color[5]);

	SDL_SetRenderDrawColor(pRenderer_, 255, 0, 211, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color[6], n_color[6]);
	
	n=0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			SDL_Rect grid { x_playzone+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects[n] = grid;
			n++;
		}
	}
}

void Window::draw_gamestate_duo(int area_p1[20][10],int area_p2[20][10] )
{
	int x_playzone_p1 = (w_/5)-(pixel_size_*5);
	int x_playzone_p2 = ((4*w_)/6)-(pixel_size_*4);
	int y_playzone = (h_ /2)-(pixel_size_*10);

	SDL_Rect playzone_rects_p1 [200];
	SDL_Rect playzone_rects_color_p1 [7][200];

	SDL_Rect playzone_rects_p2 [200];
	SDL_Rect playzone_rects_color_p2 [7][200];

	int n_p1=0;
	int n_color_p1 [7]={0};
	int n_p2=0;
	int n_color_p2 [7]={0};
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int val_case_p1=area_p1[i][j];
			int val_case_p2=area_p2[i][j];
			if (val_case_p1!=0)
			{
				SDL_Rect grid_p1 { x_playzone_p1+j*pixel_size_, y_playzone+i*pixel_size_, pixel_size_, pixel_size_ };

				playzone_rects_color_p1[val_case_p1-1][n_color_p1[val_case_p1-1]] = grid_p1;

				n_color_p1[val_case_p1-1]++;
			}
			if (val_case_p2!=0)
			{
				SDL_Rect grid_p2 { x_playzone_p2+j*pixel_size_, y_playzone+i*pixel_size_, pixel_size_, pixel_size_ };

				playzone_rects_color_p2[val_case_p2-1][n_color_p2[val_case_p2-1]] = grid_p2;

				n_color_p2[val_case_p2-1]++;
			}	
		}
	}

	// Couleurs des pièces (ptet une boucle for c'est mieux mais c'est moins pratique)
	SDL_SetRenderDrawColor(pRenderer_, 255, 255, 10, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[0], n_color_p1[0]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[0], n_color_p2[0]);


	SDL_SetRenderDrawColor(pRenderer_, 0, 255, 255, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[1], n_color_p1[1]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[1], n_color_p2[1]);


	SDL_SetRenderDrawColor(pRenderer_, 0, 255, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[2], n_color_p1[2]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[2], n_color_p2[2]);


	SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[3], n_color_p1[3]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[3], n_color_p2[3]);


	SDL_SetRenderDrawColor(pRenderer_, 0, 50, 255, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[4], n_color_p1[4]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[4], n_color_p2[4]);


	SDL_SetRenderDrawColor(pRenderer_, 255, 140, 0, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[5], n_color_p1[5]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[5], n_color_p2[5]);


	SDL_SetRenderDrawColor(pRenderer_, 255, 0, 211, 255);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p1[6], n_color_p1[6]);
	SDL_RenderFillRects(pRenderer_, playzone_rects_color_p2[6], n_color_p2[6]);

	
	n_p1=0;
	n_p2=0;

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			SDL_Rect grid_p1 { x_playzone_p1+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects_p1[n_p1] = grid_p1;
			n_p1++;
			SDL_Rect grid_p2 { x_playzone_p2+i*pixel_size_, y_playzone+j*pixel_size_, pixel_size_, pixel_size_ };
			playzone_rects_p2[n_p2] = grid_p2;
			n_p2++;
		}
	}
}


void Window::draw_ghost_piece_duo(Piece ghostPiece_p1, Piece ghostPiece_p2)
{
	int x_ghostpiece_zone_p1 = (w_/5)-(pixel_size_*5)+pixel_size_*11;
	int x_ghostpiece_zone_p2 = ((4*w_)/6)-(pixel_size_*4)+pixel_size_*11;
	
	int y_ghostpiece_zone = (h_ /2)-(pixel_size_*10);

	SDL_Rect ghostPiece_rects_p1 [12];
	SDL_Rect ghostPiece_rects_p2 [12];

	SDL_Rect ghostPieceZone_p1 {x_ghostpiece_zone_p1,y_ghostpiece_zone,pixel_size_*6,pixel_size_*6};
	SDL_Rect ghostPieceZone_p2 {x_ghostpiece_zone_p2,y_ghostpiece_zone,pixel_size_*6,pixel_size_*6};

	int n_p1=0;
	int n_p2=0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int val_case_p1=PIECES[ghostPiece_p1.getKind()][0][i][j];
			if (val_case_p1!=0)
			{
				SDL_Rect grid_p1 { x_ghostpiece_zone_p1+(j+1)*pixel_size_, y_ghostpiece_zone+(i+1)*pixel_size_, pixel_size_, pixel_size_ };

				ghostPiece_rects_p1[n_p1] = grid_p1;
				n_p1++;

			}
			int val_case_p2=PIECES[ghostPiece_p2.getKind()][0][i][j];
			if (val_case_p2!=0)
			{
				SDL_Rect grid_p2 { x_ghostpiece_zone_p2+(j+1)*pixel_size_, y_ghostpiece_zone+(i+1)*pixel_size_, pixel_size_, pixel_size_ };

				ghostPiece_rects_p2[n_p2] = grid_p2;
				n_p2++;

			}
			
		}
		
	}
	switch (ghostPiece_p1.getColor())
	{
		case 1:
			SDL_SetRenderDrawColor(pRenderer_, 255, 255, 10, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 255, 255);
			break;
		case 3:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 0, 255);
			break;
		case 4:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
			break;
		case 5:
			SDL_SetRenderDrawColor(pRenderer_, 0, 50, 255, 255);
			break;
		case 6:
			SDL_SetRenderDrawColor(pRenderer_, 255, 140, 0, 255);
			break;
		case 7:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 211, 255);
			break;
		default:
			break;
	}
	SDL_RenderFillRects(pRenderer_, ghostPiece_rects_p1, n_p1);

	switch (ghostPiece_p2.getColor())
	{
		case 1:
			SDL_SetRenderDrawColor(pRenderer_, 255, 255, 10, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 255, 255);
			break;
		case 3:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 0, 255);
			break;
		case 4:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
			break;
		case 5:
			SDL_SetRenderDrawColor(pRenderer_, 0, 50, 255, 255);
			break;
		case 6:
			SDL_SetRenderDrawColor(pRenderer_, 255, 140, 0, 255);
			break;
		case 7:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 211, 255);
			break;
		default:
			break;
	}

	SDL_RenderFillRects(pRenderer_, ghostPiece_rects_p2, n_p2);


	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);
	SDL_RenderDrawRects(pRenderer_, ghostPiece_rects_p1, n_p1);
	SDL_RenderDrawRects(pRenderer_, ghostPiece_rects_p2, n_p2);


}


void Window::draw_ghost_piece(Piece ghostPiece)
{
	int x_ghostpiece_zone = (w_/2)-(pixel_size_*5)+pixel_size_*11;
	int y_ghostpiece_zone = (h_ /2)-(pixel_size_*10);

	SDL_Rect ghostPiece_rects [12];
	SDL_Rect ghostPieceZone {x_ghostpiece_zone,y_ghostpiece_zone,pixel_size_*6,pixel_size_*6};
	int n=0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int val_case=PIECES[ghostPiece.getKind()][0][i][j];
			if (val_case!=0)
			{
				SDL_Rect grid { x_ghostpiece_zone+(j+1)*pixel_size_, y_ghostpiece_zone+(i+1)*pixel_size_, pixel_size_, pixel_size_ };

				ghostPiece_rects[n] = grid;
				n++;

			}
			
		}
		
	}
	switch (ghostPiece.getColor())
	{
		case 1:
			SDL_SetRenderDrawColor(pRenderer_, 255, 255, 10, 255);
			break;
		case 2:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 255, 255);
			break;
		case 3:
			SDL_SetRenderDrawColor(pRenderer_, 0, 255, 0, 255);
			break;
		case 4:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 0, 255);
			break;
		case 5:
			SDL_SetRenderDrawColor(pRenderer_, 0, 50, 255, 255);
			break;
		case 6:
			SDL_SetRenderDrawColor(pRenderer_, 255, 140, 0, 255);
			break;
		case 7:
			SDL_SetRenderDrawColor(pRenderer_, 255, 0, 211, 255);
			break;
		default:
			break;
	}

	SDL_RenderFillRects(pRenderer_, ghostPiece_rects, n);

	SDL_SetRenderDrawColor(pRenderer_, 100, 100, 100, 255);
	SDL_RenderDrawRects(pRenderer_, ghostPiece_rects, n);

}

void Window::draw_score(int score)
{
	if (score!=0)
	{
		int x_playzone = (w_/2)-(pixel_size_*5);
		int y_playzone = (h_ /2)-(pixel_size_*10);
		SDL_Color White = {255, 200, 200};
		std::string s= std::to_string(score);
		char const* message = s.c_str();
		surfaceText_p1 = TTF_RenderText_Solid(font,message, White); 
		textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);

		SDL_Rect Message_rect; 
		Message_rect.x = x_playzone+pixel_size_*11 +2;
		Message_rect.y = y_playzone+7*pixel_size_; 
		Message_rect.w = pixel_size_*6 -4;
		Message_rect.h = pixel_size_*2; 

		SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);
	}
	
}

void Window::draw_score_duo(int score_p1, int score_p2)
{
	if ((score_p1!=0) || (score_p2 !=0))
	{
		int x_playzone_p1 = (w_/5)-(pixel_size_*5);
		int x_playzone_p2 = ((4*w_)/6)-(pixel_size_*4);

		int y_playzone = (h_ /2)-(pixel_size_*10);
		SDL_Color White = {255, 200, 200};
		std::string s_p1= std::to_string(score_p1);
		std::string s_p2= std::to_string(score_p2);

		char const* message_p1 = s_p1.c_str();
		surfaceText_p1 = TTF_RenderText_Solid(font,message_p1, White); 
		textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);

		char const* message_p2 = s_p2.c_str();
		surfaceText_p2 = TTF_RenderText_Solid(font,message_p2, White); 
		textureText_p2 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p2);

		SDL_Rect Message_rect_p1; 
		Message_rect_p1.x = x_playzone_p1+pixel_size_*11;
		Message_rect_p1.y = y_playzone+7*pixel_size_+2; 
		Message_rect_p1.w = pixel_size_*6 -2;
		Message_rect_p1.h = pixel_size_*2; 

		SDL_Rect Message_rect_p2; 
		Message_rect_p2.x = x_playzone_p2+pixel_size_*11;
		Message_rect_p2.y = y_playzone+7*pixel_size_+2; 
		Message_rect_p2.w = pixel_size_*6 -2;
		Message_rect_p2.h = pixel_size_*2; 

		SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect_p1);
		SDL_RenderCopy(pRenderer_, textureText_p2, NULL, &Message_rect_p2);
	}
}

void Window::draw_menu(int game_state)
{
	draw_background();
	SDL_Color White = {255, 255, 255};
	SDL_Rect Message_rect;


	SDL_SetRenderDrawColor(pRenderer_, 200, 0, 200, 255);
	switch (game_state)
	{
		case 0:
			Message_rect.y = h_/3 - 105/2;
			break;
		
		case 1:
			Message_rect.y = h_/2 - 105/2;
			break;
		
		case 2:
			Message_rect.y = h_/1.5 - 105/2;
			break;
	}

	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 150);
	
	//SDL_RenderFillRect(pRenderer_, &Message_rect);

	Message_rect.x = w_/2 - 200/2;
	Message_rect.w = 200;
	Message_rect.h = 100;
	SDL_RenderFillRect(pRenderer_, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"SOLO", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/3 - 70/2;
	Message_rect.x = w_/2 -170/2; 
	Message_rect.w = 170;
	Message_rect.h = 70;
	//SDL_RenderFillRect(pRenderer_, &Message_rect);
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"DUO", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/2 - 70/2;
	Message_rect.x = w_/2 -170/2; 
	Message_rect.w = 170;
	Message_rect.h = 70;
	//SDL_RenderFillRect(pRenderer_, &Message_rect);
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"EXIT", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/1.5 - 70/2;
	Message_rect.x = w_/2 -170/2; 
	Message_rect.w = 170;
	Message_rect.h = 70;
	//SDL_RenderFillRect(pRenderer_, &Message_rect);
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	SDL_RenderPresent(pRenderer_);
}

void Window::draw_endScreen(int game_state,int area [20][10],int score)
{
	SDL_Color White = {255, 255, 255};
	SDL_Color Red = {255,0,0};
	SDL_Rect Message_rect;

	draw_background();
	draw_background_playzone();
	draw_gamestate(area);
	draw_grid();
	
	Message_rect.y = 0;
	Message_rect.x = 0;
	Message_rect.w = w_;
	Message_rect.h = h_;
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 150);
	
	SDL_RenderFillRect(pRenderer_, &Message_rect);
	
	draw_score(score);

	SDL_SetRenderDrawColor(pRenderer_, 200, 0, 200, 255);
	switch (game_state)
	{
		case 0:
			Message_rect.y = h_/2 - 100/2;
			break;
		
		case 1:
			Message_rect.y = h_/1.5 - 100/2;
			break;
	}

	Message_rect.x = w_/2 - 250/2;
	Message_rect.w = 250;
	Message_rect.h = 100;
	SDL_SetRenderDrawColor(pRenderer_, 255, 255, 255, 255);
	SDL_RenderDrawRect(pRenderer_, &Message_rect);

	Message_rect.x +=1;
	Message_rect.y +=1; 
	Message_rect.w -=2; 
	Message_rect.h -=2;
	SDL_RenderDrawRect(pRenderer_, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"GAME OVER", Red);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/3 - 100/2;
	Message_rect.x = w_/2 -300/2; 
	Message_rect.w = 300;
	Message_rect.h = 100;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"Restart", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/2 - 70/2;
	Message_rect.x = w_/2 -220/2; 
	Message_rect.w = 220;
	Message_rect.h = 70;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"Exit to menu", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/1.5 - 70/2;
	Message_rect.x = w_/2 -220/2; 
	Message_rect.w = 220;
	Message_rect.h = 70;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);


	SDL_RenderPresent(pRenderer_);
	
}

void Window::draw_endScreen_2p(int game_state, Game *g)
{
	SDL_Color White = {255, 255, 255};
	SDL_Color Green = {0,255,0};
	SDL_Rect Message_rect;

	draw_background();
	draw_background_playzone_duo();
	draw_gamestate_duo(g->board_p1->area,g->board_p2->area);
	draw_grid_duo();


	Message_rect.y = 0;
	Message_rect.x = 0;
	Message_rect.w = w_;
	Message_rect.h = h_;
	SDL_SetRenderDrawBlendMode(pRenderer_, SDL_BLENDMODE_BLEND);

	SDL_SetRenderDrawColor(pRenderer_, 0, 0, 0, 150);
	
	SDL_RenderFillRect(pRenderer_, &Message_rect);

	draw_score_duo(g->board_p1->getScore(), g->board_p2->getScore());
	
	switch (game_state)
	{
		case 0:
			Message_rect.y = h_/1.75 - 100/2;
			break;
		
		case 1:
			Message_rect.y = h_/1.35 - 100/2;
			break;
	}

	Message_rect.x = w_/2 -250/2; 
	Message_rect.w = 250;
	Message_rect.h = 90;
	SDL_SetRenderDrawColor(pRenderer_, 255, 255, 255, 255);
	SDL_RenderDrawRect(pRenderer_, &Message_rect);
	Message_rect.x +=1;
	Message_rect.y +=1; 
	Message_rect.w -=2; 
	Message_rect.h -=2;

	SDL_RenderDrawRect(pRenderer_, &Message_rect);

	if (g->board_p1->getScore() > g->board_p2->getScore())
		surfaceText_p1 = TTF_RenderText_Solid(font,"P1 WON THE DUEL !", Green);
	else if (g->board_p1->getScore() < g->board_p2->getScore())
		surfaceText_p1 = TTF_RenderText_Solid(font,"P2 WON THE DUEL !", Green);
	else
		surfaceText_p1 = TTF_RenderText_Solid(font,"IT'S A DRAW !", Green);

	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.x = w_/2 - 500/2;
	Message_rect.y = h_/3.2 -100/2; 
	Message_rect.w = 500;
	Message_rect.h = 100;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"Restart", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/1.75 - 70/2;
	Message_rect.x = w_/2 -220/2; 
	Message_rect.w = 220;
	Message_rect.h = 70;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	surfaceText_p1 = TTF_RenderText_Solid(font,"Exit to menu", White);
	textureText_p1 = SDL_CreateTextureFromSurface(pRenderer_, surfaceText_p1);
	Message_rect.y = h_/1.35 - 70/2;
	Message_rect.x = w_/2 -220/2; 
	Message_rect.w = 220;
	Message_rect.h = 70;
	SDL_RenderCopy(pRenderer_, textureText_p1, NULL, &Message_rect);

	SDL_RenderPresent(pRenderer_);
	
}


int Window::endScreen(Game* g,int nb_joueur)
{
	int quit=false;
	int game_state=0;
	// Ecran de fin
	if(nb_joueur==1)
		draw_endScreen(game_state,g->board_p1->area, g->board_p1->getScore());
	else
		draw_endScreen_2p(game_state,g);
	
	while(!quit)
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
						quit= true;
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
						game_state=1;
						quit=true;
					}
					if (state[ SDL_SCANCODE_DOWN ])		
					{
						if (game_state==1)
							game_state=0;
						else 
							game_state=1;
					}
					if (state[ SDL_SCANCODE_UP ])
					{
						if(game_state==0)
							game_state=1;
						else 
							game_state=0;
					}
					if (state[ SDL_SCANCODE_RETURN ])
					{
						quit=true;
					}
					if(nb_joueur==1)
						draw_endScreen(game_state,g->board_p1->area, g->board_p1->getScore());
					else
						draw_endScreen_2p(game_state,g);
					break;
			}
		}
	}
	return game_state;
}


void Window::update(int area [20][10],Piece ghostPiece,int score)
{
	draw_background();
	draw_background_playzone();
	draw_gamestate(area);
	draw_ghost_piece(ghostPiece);
	draw_grid();
	draw_score(score);
	SDL_RenderPresent(pRenderer_);
}

void Window::update_duo(int area_p1 [20][10],int area_p2 [20][10] ,Piece ghostPiece_p1, Piece ghostPiece_p2,int score_p1, int score_p2)
{

	draw_background();
	draw_background_playzone_duo();
	draw_gamestate_duo(area_p1,area_p2);
	draw_ghost_piece_duo(ghostPiece_p1,ghostPiece_p2);
	draw_grid_duo();
	draw_score_duo(score_p1, score_p2);
	SDL_RenderPresent(pRenderer_);
}