#include "ai.h"
#include <cstdlib>
#include <ctime>
#include <limits>

AI::AI(gameBoard brd, int knd, int lvl, int dpth)
{
    gBoard = brd;
    kind = knd;
    level = lvl;
    depth = dpth;

    srand(time(nullptr));

}

AI::AI(int knd, int lvl, int dpth)
{
    kind = knd;
    level = lvl;
    depth = dpth;

    srand(time(nullptr));

}

AI::~AI()
{
    for(auto mv : pMoves)
        delete mv;
    for(auto chB : pChildBoards)
        delete chB;
}

void AI::initialize()
{
    for(auto mv : pMoves)
        delete mv;
    for(auto chB : pChildBoards)
        delete chB;

    pMoves.clear();
    pChildBoards.clear();
}

void AI::setMoves()
{
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2) && gBoard.getBox(j, i) == 1*kind*6){
                setKingMoves(j, i);
            }
            if (!((i+j)%2) && gBoard.getBox(j, i) == 1*kind){
                if (j - 1 > 0 && i - 1*kind > 0 && i - 1*kind < 9 && gBoard.getBox(j - 1, i - 1*kind) == 0)
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j-1, i-1*kind), kind));
                if (j + 1 < 9 && i - 1*kind > 0 && i - 1*kind < 9 && gBoard.getBox(j + 1, i - 1*kind) == 0)
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j+1, i-1*kind), kind));
            }
        }
    }
}

void AI::setJumps()
{
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2) && gBoard.getBox(j, i) == 1*kind*6){
                setKingJumps(j, i);
            }
            if (!((i+j)%2) && gBoard.getBox(j, i) == 1*kind){
                if (j - 2 > 0 && i - 2 > 0 && gBoard.getBox(j - 2, i - 2) == 0 && (gBoard.getBox(j - 1, i - 1) == (-1)*kind || gBoard.getBox(j - 1, i - 1) == (-1)*kind*6))
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j-2, i-2), kind, true, QPoint(j-1, i-1)));
                if (j + 2 < 9 && i - 2 > 0 && gBoard.getBox(j + 2, i - 2) == 0 && (gBoard.getBox(j + 1, i - 1) == (-1)*kind || gBoard.getBox(j + 1, i - 1) == (-1)*kind*6))
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j+2, i-2), kind, true, QPoint(j+1, i-1)));
                if (j - 2 > 0 && i + 2 < 9 && gBoard.getBox(j - 2, i + 2) == 0 && (gBoard.getBox(j - 1, i + 1) == (-1)*kind || gBoard.getBox(j - 1, i + 1) == (-1)*kind*6))
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j-2, i+2), kind, true, QPoint(j-1, i+1)));
                if (j + 2 < 9 && i + 2 < 9 && gBoard.getBox(j + 2, i + 2) == 0 && (gBoard.getBox(j + 1, i + 1) == (-1)*kind || gBoard.getBox(j + 1, i + 1) == (-1)*kind*6))
                    pMoves.push_back(new dMove(QPoint(j, i), QPoint(j+2, i+2), kind, true, QPoint(j+1, i+1)));
            }
        }
    }
}

void AI::setKingMoves(int x, int y)
{
    for(int i = 1, j = 1; x + j < 9 && y + i < 9; i++, j++){
        if (gBoard.getBox(x+j, y+i) != 0) break;
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+j, y+i), kind));
    }

    for(int i = 1, j = 1; x - j > 0 && y + i < 9; i++, j++){
        if (gBoard.getBox(x-j, y+i) != 0) break;
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-j, y+i), kind));
    }

    for(int i = 1, j = 1; x + j < 9 && y - i > 0; i++, j++){
        if (gBoard.getBox(x+j, y-i) != 0) break;
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+j, y-i), kind));
    }

    for(int i = 1, j = 1; x - j > 0 && y - i > 0; i++, j++){
        if (gBoard.getBox(x+j, y+i) != 0) break;
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-j, y-i), kind));
    }
}

void AI::setKingJumps(int x, int y)
{
    for(int i = 1, j = 1; x + j < 9 && y + i < 9; i++, j++){
        if (gBoard.getBox(x+j, y+i) == kind || gBoard.getBox(x+j, y+i) == kind*6) break;
        if (gBoard.getBox(x+j, y+i) == kind*(-1) || gBoard.getBox(x+j, y+i) == kind*6*(-1)){
            for (int d = 1; d < 8; d++){
                if (x+j+d > 8 || y+i+d > 8 || gBoard.getBox(x+j+d, y+i+d) != 0) break;
                pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+j+d, y+i+d), kind, true, QPoint(x+j, y+i)));
            }
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y + i < 9; i++, j++){
        if (gBoard.getBox(x-j, y+i) == kind || gBoard.getBox(x-j, y+i) == kind*6) break;
        if (gBoard.getBox(x-j, y+i) == kind*(-1) || gBoard.getBox(x-j, y+i) == kind*6*(-1)){
            for (int d = 1; d < 8; d++){
                if (x-j-d < 1 || y+i+d > 8 || gBoard.getBox(x-j-d, y+i+d) != 0) break;
                pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-j-d, y+i+d), kind, true, QPoint(x-j, y+i)));
            }
        }
    }

    for(int i = 1, j = 1; x + j < 9 && y - i > 0; i++, j++){
        if (gBoard.getBox(x+j, y-i) == kind || gBoard.getBox(x+j, y-i) == kind*6) break;
        if (gBoard.getBox(x+j, y-i) == kind*(-1) || gBoard.getBox(x+j, y-i) == kind*6*(-1)){
            for (int d = 1; d < 8; d++){
                if (x+j+d > 8 || y-i-d < 1 || gBoard.getBox(x+j+d, y-i-d) != 0) break;
                pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+j+d, y-i-d), kind, true, QPoint(x+j, y-i)));
            }
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y - i > 0; i++, j++){
        if (gBoard.getBox(x-j, y-i) == kind || gBoard.getBox(x-j, y-i) == kind*6) break;
        if (gBoard.getBox(x-j, y-i) == kind*(-1) || gBoard.getBox(x-j, y-i) == kind*6*(-1)){
            for (int d = 1; d < 8; d++){
                if (x-j-d < 1 || y-i-d < 1 || gBoard.getBox(x-j-d, y-i-d) != 0) break;
                pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-j-d, y-i-d), kind, true, QPoint(x-j, y-i)));
            }
        }
    }
}

void AI::setChainJumps(int x, int y)
{
    if (x - 2 > 0 && y - 2 > 0 && gBoard.getBox(x - 2, y - 2) == 0 && (gBoard.getBox(x - 1, y - 1) == (-1)*kind || gBoard.getBox(x - 1, y - 1) == (-1)*kind*6))
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-2, y-2), kind, true, QPoint(x-1, y-1)));
    if (x + 2 < 9 && y - 2 > 0 && gBoard.getBox(x + 2, y - 2) == 0 && (gBoard.getBox(x + 1, y - 1) == (-1)*kind || gBoard.getBox(x + 1, y - 1) == (-1)*kind*6))
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+2, y-2), kind, true, QPoint(x+1, y-1)));
    if (x - 2 > 0 && y + 2 < 9 && gBoard.getBox(x - 2, y + 2) == 0 && (gBoard.getBox(x - 1, y + 1) == (-1)*kind || gBoard.getBox(x - 1, y + 1) == (-1)*kind*6))
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x-2, y+2), kind, true, QPoint(x-1, y+1)));
    if (x + 2 < 9 && y + 2 < 9 && gBoard.getBox(x + 2, y + 2) == 0 && (gBoard.getBox(x + 1, y + 1) == (-1)*kind || gBoard.getBox(x + 1, y + 1) == (-1)*kind*6))
        pMoves.push_back(new dMove(QPoint(x, y), QPoint(x+2, y+2), kind, true, QPoint(x+1, y+1)));
}

void AI::updateScores()
{
    if (pChildBoards.empty()){
        avgScore = gBoard.evaluate();
        return;
    }

    for(auto chB : pChildBoards)
        chB->updateScores();

    float score {0.0};
    for (auto chB : pChildBoards)
        score += chB->avgScore;

    avgScore = score / pChildBoards.size();
}

unsigned AI::getOutcomes()
{
    initialize();

    if (level == depth) return 0;

    setJumps();
    if (pMoves.empty())
        setMoves();
    if (pMoves.empty())
        return 0;

    unsigned nOutcomes = pMoves.size();
    bool kingMade = false;

    for (unsigned i = 0; i < nOutcomes; i++){
        gBoard.makeMove(pMoves[i]);
        if ((pMoves[i]->destination.y() == 1 && gBoard.getBox(pMoves[i]->destination.x(), pMoves[i]->destination.y()) == 1) || (pMoves[i]->destination.y() == 8 && gBoard.getBox(pMoves[i]->destination.x(), pMoves[i]->destination.y()) == -1)){
            kingMade = true;
            gBoard.setBox(pMoves[i]->destination, kind*6);
        }
        pChildBoards.push_back(new AI(gBoard, (-1)*kind, level + 1, depth));
        gBoard.unmakeMove(pMoves[i]);
        if (kingMade){
            gBoard.setBox(pMoves[i]->source, kind);
            kingMade = false;
        }
    }

    for (auto chB : pChildBoards)
        chB->getOutcomes();

    return nOutcomes;
}

dMove AI::getRandMove()
{
    initialize();

    setJumps();
    if (pMoves.empty())
        setMoves();
    if (pMoves.empty()){
        return dMove(QPoint(0,0), QPoint(0,0), 0);
    }

    unsigned n = pMoves.size();
    int i = rand()%n;

    return *pMoves[i];
}

dMove AI::getChainJump(int x, int y)
{
    initialize();
    setChainJumps(x, y);

    if (pMoves.empty()){
        return dMove(QPoint(0,0), QPoint(0,0), 0);
    }

    unsigned n = pMoves.size();
    int i = rand()%n;

    return *pMoves[i];
}

dMove AI::getKingChainJump(int x, int y)
{
    initialize();
    setKingJumps(x, y);

    if (pMoves.empty()){
        return dMove(QPoint(0,0), QPoint(0,0), 0);
    }

    unsigned n = pMoves.size();
    int i = rand()%n;

    return *pMoves[i];
}

dMove AI::getDecision()
{
    unsigned nChildren = getOutcomes();
    updateScores();
    if (nChildren == 0) {
        return dMove(QPoint(0,0), QPoint(0,0), 0);
    }

    unsigned favored {0};
    float bestAvg {0.0};

    //bestAvg = std::numeric_limits<float>::min();
    bestAvg = -1000;

    for (unsigned i = 0; i < nChildren; i++){
        if(pChildBoards[i]->avgScore > bestAvg) {
            bestAvg = pChildBoards[i]->avgScore;
            favored = i;
        }
    }

    return *pMoves[favored];
}
