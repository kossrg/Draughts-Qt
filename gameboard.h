#ifndef GAMEBOARD_H
#define GAMEBOARD_H
#include <QPoint>
#include "move.h"


class gameBoard             //numerical representation of game board
{
private:
    short int box[8][8];

public:
    gameBoard();

    void initialize();      //sets initial position for the game start

    void makeMove(dMove *m);
    void unmakeMove(dMove *m);

    int evaluate();         //evaluates position for AI

    void setBox(int x, int y, short v);
    void setBox(QPoint pnt, short v);
    int getBox(int x, int y);
    int getBox(QPoint pnt);

    bool checkForMove(int k);
    bool checkForJump(int k);
    bool checkForKingMove(int j, int i, int k);
    bool checkForKingJump(int j, int i, int k);
    bool checkForMoveJump(int k);
    bool checkForChainJump(int j, int i, int k);

    //gameBoard operator=(gameBoard br);
};

#endif // GAMEBOARD_H
