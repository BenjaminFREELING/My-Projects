#include "board.h"
 
void Board::flood(int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE])
{
    if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
        return;
 
    visited[px][py] = true;
    area[i][j] = value;
 
    flood(i, j - 1, px, py - 1, k, o, value, visited);
    flood(i + 1, j, px + 1, py, k, o, value, visited);
    flood(i, j + 1, px, py + 1, k, o, value, visited);
    flood(i - 1, j, px - 1, py, k, o, value, visited);
}
 
void Board::flood(int i, int j, int px, int py, int k, int o, bool &flag, bool visited[][SIZE])
{
    if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
        return;
 
    visited[px][py] = true;
 
    if(i < 0 || i >= BOARD_HEIGHT || j < 0 || j >= BOARD_WIDTH || area[i][j] != FREE )
    {
        flag = false;
        return;
    }
 
    flood(i, j - 1, px, py - 1, k, o, flag, visited);
    flood(i + 1, j, px + 1, py, k, o, flag, visited);
    flood(i, j + 1, px, py + 1, k, o, flag, visited);
    flood(i - 1, j, px - 1, py, k, o, flag, visited);
}
 
void Board::floodFill(int i, int j, int px, int py, int k, int o, int value)
{
    bool visited[SIZE][SIZE];
 
    for(int l = 0; l < SIZE; ++l)
        for(int m = 0; m < SIZE; ++m)
            visited[l][m] = false;
 
    flood(i, j, px, py, k, o, value, visited);
}
 
Board::Board()
{
    for(int i = 0; i < BOARD_HEIGHT; ++i)
        for(int j = 0; j < BOARD_WIDTH; ++j)
            area[i][j] = FREE;

    score=0;
}

Piece Board::createRandomPiece()
{
    int k = std::rand()%7;
    int o = 0;
    Piece *p = new Piece(k,o);
    p->setColor(k+1);
    return *p;
}


void Board::printBoard(int move_cursor)
{
    //if(move_cursor)
    //    std::cout<<"\e[23A";
    for(int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for(int j = 0; j < BOARD_WIDTH; ++j)
        {
            std::cout<< area[i][j] << " ";
        }
        std::cout<<std::endl;
    }       
    std::cout<<std::endl;

}

void Board::setCurrentPiece(Piece p)
{
    currentPiece= p;
}

void Board::ghostToCurrent()
{
    setCurrentPiece(getGhostPiece());
    setGhostPiece(createRandomPiece());
    newPiece(getCurrentPiece());
}

void Board::setGhostPiece(Piece p)
{
    ghostPiece = p;
}

Piece Board::getGhostPiece()
{
    return ghostPiece;
}

Piece Board::getCurrentPiece()
{
    return currentPiece;
}

void Board::setScore(int s)
{
    score = s;
}

int Board::getScore()
{
    return score;
}
 
void Board::drawPiece(Piece p)
{
    int i = p.getPosX();
    int j = p.getPosY();
 
    int k = p.getKind();
    int o = p.getOrientation();
 
    floodFill(i, j, PIVOT_X, PIVOT_Y, k, o, p.getColor());
}
 
void Board::clearPiece(Piece p)
{
    int i = p.getPosX();
    int j = p.getPosY();
 
    int k = p.getKind();
    int o = p.getOrientation();
 
    floodFill(i, j, PIVOT_X, PIVOT_Y, k, o, FREE);
}

bool Board::isPieceSwpanable(Piece p)
{
    p.setPosX(ORIGIN_X);
    p.setPosY(ORIGIN_Y);
    int j_ancre=0,i_ancre=0;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (PIECES[p.getKind()][p.getOrientation()][i][j]==2)
            {
                i_ancre=i;
                j_ancre=j;
            }
            
        }
        
    }

    //  std::cout<<" i_ancre= "<<i_ancre << " j_ancre= "<<j_ancre<<std::endl;
    for (int i = 0+i_ancre; i < SIZE; i++)
    {
        for (int j = 0 ; j < SIZE; j++)
        {
           //std::cout << " i= "<<ORIGIN_X+i-i_ancre<< " j= " <<ORIGIN_Y-j_ancre+j<<std::endl;
            //std::cout << area[ ORIGIN_X+i-i_ancre][ORIGIN_Y-j_ancre+j]<<"-"<<PIECES[p.getKind()][p.getOrientation()][i][j]<<" ";
            if ((area[ ORIGIN_X+i-i_ancre][ORIGIN_Y-j_ancre+j]!=0)&&(PIECES[p.getKind()][p.getOrientation()][i][j]!=0))
            {
                return false;
            }
        }
        
    }
    return true;
}



void Board::newPiece(Piece p)
{
    p.setPosX(ORIGIN_X);
    p.setPosY(ORIGIN_Y);
 
    drawPiece(p);
 
    setCurrentPiece(p);
}
 
bool Board::isCurrentPieceMovable(int x, int y)
{
    clearPiece(currentPiece);
 
    bool movable = true;
 
    bool visited[SIZE][SIZE];
 
    for(int l = 0; l < SIZE; ++l)
        for(int m = 0; m < SIZE; ++m)
            visited[l][m] = false;
 
    int k = currentPiece.getKind();
    int o = currentPiece.getOrientation();
 
    flood(x, y, PIVOT_X, PIVOT_Y, k, o, movable, visited);
 
    drawPiece(currentPiece);
 
    return movable;
}
 
bool Board::isCurrentPieceRotable(int o)
{
    clearPiece(currentPiece);
 
    bool rotable = true;
 
    bool visited[SIZE][SIZE];
 
    for(int i = 0; i < SIZE; ++i)
        for(int j = 0; j < SIZE; ++j)
            visited[i][j] = false;
 
    int k = currentPiece.getKind();
 
    flood(currentPiece.getPosX(), currentPiece.getPosY(), PIVOT_X, PIVOT_Y, k, o, rotable, visited);
 
    drawPiece(currentPiece);
 
    return rotable;
}
 
void Board::moveCurrentPieceDown()
{
    int x = currentPiece.getPosX();
    int y = currentPiece.getPosY();
 
    if(isCurrentPieceMovable(x + 1, y))
    {
        clearPiece(currentPiece);
        currentPiece.setPosX(x + 1);
 
        drawPiece(currentPiece);
    }
}
 
void Board::moveCurrentPieceLeft()
{
    int x = currentPiece.getPosX();
    int y = currentPiece.getPosY();
 
    if(isCurrentPieceMovable(x, y - 1))
    {
        clearPiece(currentPiece);
        currentPiece.setPosY(y - 1);
 
        drawPiece(currentPiece);
    }
}
 
void Board::moveCurrentPieceRight()
{
    int x = currentPiece.getPosX();
    int y = currentPiece.getPosY();
 
    if(isCurrentPieceMovable(x, y + 1))
    {
        clearPiece(currentPiece);
        currentPiece.setPosY(y + 1);
 
        drawPiece(currentPiece);
    }
}
 
void Board::rotateCurrentPieceLeft()
{
    int o = currentPiece.getOrientation();
 
    if(o > 0)
        o--;
    else
        o = NB_ROTATIONS - 1;
 
    if(isCurrentPieceRotable(o))
    {
        clearPiece(currentPiece);
 
        currentPiece.setOrientation(o);
        drawPiece(currentPiece);
    }
}
 
void Board::rotateCurrentPieceRight()
{
    int o = currentPiece.getOrientation();
 
    if(o < NB_ROTATIONS - 1)
        o++;
    else
        o = 0;
 
    if(isCurrentPieceRotable(o))
    {
        clearPiece(currentPiece);
        currentPiece.setOrientation(o);
 
        drawPiece(currentPiece);
    }
}
 
void Board::deleteLine(int y)
{
 
    for(int i = y; i > 0; i--)
    {
        for(int j = 0; j < BOARD_WIDTH; j++)
            area[i][j] = area[i-1][j];
    }
 
}
 
int Board::deletePossibleLines()
{
    int n_line_deleted = 0;
    int n_affile=0;
    for(int i = 0; i < BOARD_HEIGHT; ++i)
    {
        for(int j = 0; j < BOARD_WIDTH; ++j)
        {
            if (area[i][j]!=0)
            {
                n_affile++;
            }
            
        }
        if (n_affile==BOARD_WIDTH)
        {
            n_line_deleted++;
            deleteLine(i);
        }
        n_affile=0;
    }       
    return n_line_deleted;
}

void Board::add_lines(int n_lines)
{
    clearPiece(currentPiece);
    for(int n=0 ; n < n_lines; n++)
    {
        for(int i = 1; i <BOARD_HEIGHT; i++)
        {
            for(int j = 0; j < BOARD_WIDTH; j++)
                area[i-1][j] = area[i][j];
        }
        int trou = std::rand()%BOARD_WIDTH;
        for(int j = 0; j < BOARD_WIDTH; j++)
        {
            if(j!=trou)
                area[BOARD_HEIGHT-1][j] = (std::rand()%6)+1;
            else
                area[BOARD_HEIGHT-1][j] = FREE;  
        }
    }
    int n = currentPiece.getPosX();
    if (n=!0)
        currentPiece.setPosX(n-1);
    drawPiece(currentPiece);
}
 
void Board::dropCurrentPiece()
{
    int x = currentPiece.getPosX();
    int y = currentPiece.getPosY();
 
    while(isCurrentPieceMovable(x++, y))
        moveCurrentPieceDown();
}
 
bool Board::isCurrentPieceFallen()
{
    int x = currentPiece.getPosX();
    int y = currentPiece.getPosY();
 
    if(isCurrentPieceMovable(x + 1, y))
        return false;
 
    return true;
}
 
void Board::clear()
{
    for(int i = 0; i < BOARD_WIDTH; ++i)
    {
        for(int j = 0; j < BOARD_HEIGHT; ++j)
            area[i][j] = FREE;
    }
}