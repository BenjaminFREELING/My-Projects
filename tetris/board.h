/* board.h */
#ifndef BOARD_H
#define BOARD_H
 
#include "piece.h"
#include <cstdlib>
#include <iostream>
 
enum  { FREE, FILLED };
 
const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 10;
 
const int ORIGIN_X = 0;
const int ORIGIN_Y = 5;
 
/**
 * @class Board
 * @brief Contient la gestion du plateau de jeu du tetris
 * */

class Board
{
private:
    Piece currentPiece; // pièce en cours de jeu
    Piece ghostPiece; // prochaine pièce a venir
    int score; // Score de la partie

/**
 * Fonction Flood Fill utilisée pour la gestion des formes des pièces lors de 
 * leur incorporation a la matrice
 * inspirée de recherches sur internet sur cet algorithme
 * */ 
    void flood(int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE]);
    void flood(int i, int j, int px, int py, int k, int o, bool &flag, bool visited[][SIZE]);
    void floodFill(int i, int j, int px, int py, int k, int o, int value);
 
public:
    /** @brief @brief Matrice représentant le jeu*/
    int area[BOARD_HEIGHT][BOARD_WIDTH];
 
    Board();

    int getScore();
    void setScore(int s);

    /** @brief Affichage en mode terminal de l'état de la matrice*/
    void printBoard(int move_cursor);    
    
    void setGhostPiece(Piece p);
    Piece getGhostPiece();

    /** @brief Génération aléatoire d'une piece de Tetris*/
    Piece createRandomPiece();

    void setCurrentPiece(Piece p);
    Piece getCurrentPiece();
 
    /** @brief transfert de la Ghost piece vers la piece courante 
     * (comprend regénération d'une nouvelle ghost piece) */
    void ghostToCurrent();

    /** @brief incorpore une pièce a la matrice*/
    void drawPiece(Piece p);
    /** @brief efface une pièce de la matrice*/
    void clearPiece(Piece p);
 
    /** @brief Verifie si une pièce peut apparaitre dans la matrice*/
    bool isPieceSwpanable(Piece p);
    /** @brief fait spawn la pièce dans la matrice (première apparition)*/
    void newPiece(Piece p);
 
    /** @brief verifcateurs des mouvements possibles de la pièce courante*/
    bool isCurrentPieceMovable(int x, int y);
    bool isCurrentPieceRotable(int o);
 
    /** @brief Fonctions faisant bouger la pièce courante*/
    void moveCurrentPieceDown();
    void moveCurrentPieceLeft();
    void moveCurrentPieceRight();
 
    /** @brief fonctions faisant tourner la pièce courante*/
    void rotateCurrentPieceLeft();
    void rotateCurrentPieceRight();
 
    /** @brief efface une ligne de la matrice*/
    void deleteLine(int y);

    /** @briefefface toutes les lignes complètes de la matrice*/
    int deletePossibleLines();
    /** @briefAjoute une ligne en bas de matrice (pour mode 2 joueurs)*/
    void add_lines(int n_lines);
 
    /** @brief Fait tomber la pièce courante*/
    void dropCurrentPiece();
    
    /** @brief verificateur de si la pièce courante est posée ou non*/
    bool isCurrentPieceFallen();
    /** @brief Remet la matrice à 0*/
    void clear();
};
 
#endif