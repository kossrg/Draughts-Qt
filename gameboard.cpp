#include "gameboard.h"

gameBoard::gameBoard()
{

}

void gameBoard::initialize()
{
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if ((i+j)%2){
                box[j-1][i-1] = 4;              //4 = forbidden (white) square
            }
            else {
                if (i > 5) {
                    box[j-1][i-1] = 1;          // 1 = black, 6 = black king
                }
                else if (i > 3) {
                    box[j-1][i-1] = 0;          //0 = vacant square
                }
                else {
                    box[j-1][i-1] = -1;         //-1 = white, -6 = white king
                }
            }
        }
    }
}

void gameBoard::makeMove(dMove *m)
{
    if (box[m->destination.x() - 1][m->destination.y() - 1] == 0){
        box[m->destination.x() - 1][m->destination.y() - 1] = box[m->source.x() - 1][m->source.y() - 1];
        box[m->source.x() - 1][m->source.y() - 1] = 0;
        if (m->jump){
            if (box[m->target.x() - 1][m->target.y() - 1] == m->kind*(-1)){
                box[m->target.x() - 1][m->target.y() - 1] = 0;
            }
            if (box[m->target.x() - 1][m->target.y() - 1] == m->kind*6*(-1)){
                box[m->target.x() - 1][m->target.y() - 1] = 0;
                m->kingTaken = true;
            }
        }
    }
}

void gameBoard::unmakeMove(dMove *m)
{
    if (box[m->source.x() - 1][m->source.y() - 1] == 0) {
        box[m->source.x() - 1][m->source.y() - 1] = box[m->destination.x() - 1][m->destination.y() - 1];
        box[m->destination.x() - 1][m->destination.y() - 1] = 0;
        if (m->jump){
            if (m->kingTaken){
                box[m->target.x() - 1][m->target.y() - 1] = m->kind*(-1)*6;
            }
            else {
                box[m->target.x() - 1][m->target.y() - 1] = m->kind*(-1);
            }
        }
    }
}

int gameBoard::evaluate()
{
    int s {0};
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2)){
                s += box[j-1][i-1];
            }
        }
    }
    return s;
}

int gameBoard::getBox(int x, int y)
{
    return box[x-1][y-1];
}

int gameBoard::getBox(QPoint pnt)
{
    return box[pnt.x()-1][pnt.y()-1];
}

void gameBoard::setBox(int x, int y, short v)
{
    box[x-1][y-1] = v;
}

void gameBoard::setBox(QPoint pnt, short v)
{
    box[pnt.x()-1][pnt.y()-1] = v;
}

bool gameBoard::checkForMove(int k)
{
    for (int i = 7; i >= 0; i--){
        for (int j = 0; j < 8; j++){
            if (!((i+j)%2) && box[j][i] == 1*k*6){
                if (checkForKingMove(j, i, k)) return true;
            }
            if (!((i+j)%2) && box[j][i] == 1*k){
                if (j - 1 >= 0 && i - 1*k >= 0 && i - 1*k < 8 && box[j-1][i-1*k] == 0)
                    return true;
                if (j + 1 < 8 && i - 1*k >= 0 && i - 1*k < 8 && box[j+1][i-1*k] == 0)
                    return true;
            }
        }
    }
    return false;
}

bool gameBoard::checkForKingMove(int j, int i, int k)
{
    if (j+1 < 8 && i+1 < 8 && box[j+1][i+1] == 0) return true;
    if (j-1 >= 0 && i+1 < 8 && box[j-1][i+1] == 0) return true;
    if (j+1 < 8 && i-1 >= 0 && box[j+1][i-1] == 0) return true;
    if (j-1 >= 0 && i-1 >= 0 && box[j-1][i-1] == 0) return true;

    return false;
}

bool gameBoard::checkForJump(int k)
{
    for (int i = 7; i >= 0; i--){
        for (int j = 0; j < 8; j++){
            if (!((i+j)%2) && box[j][i] == 1*k*6){
                 if (checkForKingJump(j, i, k)) return true;
            }
            if (!((i+j)%2) && box[j][i] == 1*k){
                if (j - 2 >= 0 && i - 2 >= 0 && box[j-2][i-2] == 0 && (box[j-1][i-1] == (-1)*k || box[j-1][i-1] == (-1)*k*6))
                    return true;
                if (j + 2 < 8 && i - 2 >= 0 && box[j+2][i-2] == 0 && (box[j+1][i-1] == (-1)*k || box[j+1][i-1] == (-1)*k*6))
                    return true;
                if (j - 2 >= 0 && i + 2 < 8 && box[j-2][i+2] == 0 && (box[j-1][i+1] == (-1)*k || box[j-1][i+1] == (-1)*k*6))
                    return true;
                if (j + 2 < 8 && i + 2 < 8 && box[j+2][i+2] == 0 && (box[j+1][i+1] == (-1)*k || box[j+1][i+1] == (-1)*k*6))
                    return true;
            }
        }
    }
    return false;
}

bool gameBoard::checkForChainJump(int j, int i, int k)
{
    j -= 1;
    i -= 1;

    if (j - 2 >= 0 && i - 2 >= 0 && box[j-2][i-2] == 0 && (box[j-1][i-1] == (-1)*k || box[j-1][i-1] == (-1)*k*6))
        return true;
    if (j + 2 < 8 && i - 2 >= 0 && box[j+2][i-2] == 0 && (box[j+1][i-1] == (-1)*k || box[j+1][i-1] == (-1)*k*6))
        return true;
    if (j - 2 >= 0 && i + 2 < 8 && box[j-2][i+2] == 0 && (box[j-1][i+1] == (-1)*k || box[j-1][i+1] == (-1)*k*6))
        return true;
    if (j + 2 < 8 && i + 2 < 8 && box[j+2][i+2] == 0 && (box[j+1][i+1] == (-1)*k || box[j+1][i+1] == (-1)*k*6))
        return true;

    return false;
}

bool gameBoard::checkForKingJump(int j, int i, int k)
{
    for(int x = 1, y = 1; j + x < 8 && i + y < 8; x++, y++){
        if (box[j+x][i+y] == k || box[j+x][i+y] == k*6) break;
        if (box[j+x][i+y] == k*(-1) || box[j+x][i+y] == k*6*(-1)){
            if (j+x+1 < 8 && i+y+1 < 8 && box[j+x+1][i+y+1] == 0)
                return true;
        }
    }

    for(int x = 1, y = 1; j - x >= 0 && i + y < 8; x++, y++){
        if (box[j-x][i+y] == k || box[j-x][i+y] == k*6) break;
        if (box[j-x][i+y] == k*(-1) || box[j-x][i+y] == k*6*(-1)){
            if (j-x-1 < 8 && i+y+1 < 8 && box[j-x-1][i+y+1] == 0)
                return true;
        }
    }

    for(int x = 1, y = 1; j + x < 8 && i - y >= 0; x++, y++){
        if (box[j+x][i-y] == k || box[j+x][i-y] == k*6) break;
        if (box[j+x][i-y] == k*(-1) || box[j+x][i-y] == k*6*(-1)){
            if (j+x+1 < 8 && i-y-1 < 8 && box[j+x+1][i-y-1] == 0)
                return true;
        }
    }

    for(int x = 1, y = 1; j - x >= 0 && i - y >= 0; x++, y++){
        if (box[j-x][i-y] == k || box[j-x][i-y] == k*6) break;
        if (box[j-x][i-y] == k*(-1) || box[j-x][i-y] == k*6*(-1)){
            if (j-x-1 < 8 && i-y-1 < 8 && box[j-x-1][i-y-1] == 0)
                return true;
        }
    }

    return false;
}

bool gameBoard::checkForMoveJump(int k)
{
    if (checkForMove(k) || checkForJump(k)) return true;
    else return false;
}
/*
gameBoard gameBoard::operator=(gameBoard br)
{
    gameBoard temp;
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            temp.box[j][i] = br.box[j][i];
        }
    }
    return temp;
}
*/
