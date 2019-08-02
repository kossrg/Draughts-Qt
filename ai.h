#ifndef AI_H
#define AI_H

#include "gameboard.h"
#include "move.h"

#include <vector>


class AI
{
private:
    void setJumps();
    void setMoves();

    void setKingJumps(int x, int y);
    void setKingMoves(int x, int y);

    void setChainJumps(int x, int y);

    unsigned getOutcomes();
    void updateScores();

    void initialize();


public:
    AI(gameBoard brd, int knd, int lvl, int dpth = 3);
    AI(int knd, int lvl, int dpth = 3);
    ~AI();

    dMove getRandMove();

    dMove getDecision();

    dMove getChainJump(int x, int y);
    dMove getKingChainJump(int x, int y);

    std::vector <dMove*> pMoves;
    std::vector <AI*> pChildBoards;

    gameBoard gBoard;

    float avgScore;

    int depth = 3;
    int level;

    int kind = 1;  //1 = AI plays for black / -1 = AI plays for white

};

#endif // AI_H
