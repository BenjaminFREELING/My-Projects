/* piece.h */
#ifndef PIECE_H
#define PIECE_H
 
#include "shapes.h"
  
/* Les coordonnées du point de pivot de la pièce */
const int PIVOT_X = 1;
const int PIVOT_Y = 2;
 
class Piece
{
 
private:
    int kind; // Le type de la pièce
    int orientation; // Son orientation (sa rotation courante)
    int color; // Sa couleur
 
    int posX; // Son ordonnée dans l'aire de jeu
    int posY; // Son abscisse dans l'aire de jeu
 
public:
    Piece();
    Piece(int k, int o);
    Piece(const Piece &p);
 
    // Ici On ne trouve que des Geters et des Seters
    void setKind(int k);
    void setOrientation(int o);
 
    int getKind();
    int getOrientation();
 
    void setColor(int c);
    int getColor();
 
    void setPosX(int x);
    int getPosX();
 
    void setPosY(int y);
    int getPosY();
};
 
#endif