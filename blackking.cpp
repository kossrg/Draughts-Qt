#include "blackking.h"

blackKing::blackKing(QPoint p, QGraphicsItem *parent)  : abstractPiece (parent)
{
    setBPosition(p);
    setWPosition();

    QPixmap pix(":/res/piece_bk.png");
    pix.setMask(pix.createMaskFromColor(QColor(255, 255, 255)));
    setPixmap(pix);

    kind = 1;
}

bool blackKing::checkLegitMove(QPoint pnt)
{
    if (pnt.x() < 1 || pnt.x() > 8) return false;
    if (pnt.y() < 1 || pnt.x() > 8) return false;

    if (parentBoard->getBox(pnt.x(), pnt.y())) return false;

    QPoint bp = this->getBPosition();

    int dy = pnt.y() - bp.y();
    int dx = pnt.x() - bp.x();

    if (dx == 0 || dy == 0) return false;

    //check if no own pieces on the way
    if (dy > 0) { //forward
        if (dx > 0) {  //right
            for (int x = 1; x < dx; x++){
                if (parentBoard->getBox(bp.x() + x, bp.y() + x) == kind || parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*6) {
                    return false;
                }
            }
        }
        else { //left
            for (int x = -1; x > dx; x--){
                if (parentBoard->getBox(bp.x() + x, bp.y() - x) == kind || parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*6) {
                    return false;
                }
            }
        }
    }
    if (dy < 0){ //backward
        if (dx > 0) { //right
            for (int x = 1; x < dx; x++){
                if (parentBoard->getBox(bp.x() + x, bp.y() - x) == kind || parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*6) {
                    return false;
                }
            }
        }
        else { //left
            for (int x = -1; x > dx; x--){
                if (parentBoard->getBox(bp.x() + x, bp.y() + x) == kind || parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*6) {
                    return false;
                }
            }
        }
    }


    //check jumps, only one piece per jump
    int nPieces {0};
    int xE {0};
    int yE {0};
    if (dy > 0) { //forward
        if (dx > 0) {  //right
            for (int x = 1; x < dx; x++){
                if (parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*(-1) || parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*(-1)*6) {
                    nPieces++;
                    xE = bp.x() + x;
                    yE = bp.y() + x;
                }
            }
            if (nPieces == 1) {
                emit this->takePiece(xE, yE, 1);
                chainJumper = true;
                return true;
            }
            else if (nPieces > 1) {
                return false;
            }
        }
        else { //left
            for (int x = -1; x > dx; x--){
                if (parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*(-1) || parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*(-1)*6) {
                    nPieces++;
                    xE = bp.x() + x;
                    yE = bp.y() - x;
                }
            }
            if (nPieces == 1) {
                emit this->takePiece(xE, yE, 1);
                chainJumper = true;
                return true;
            }
            else if (nPieces > 1) {
                return false;
            }
        }
    }
    if (dy < 0){ //backward
        if (dx > 0) { //right
            for (int x = 1; x < dx; x++){
                if (parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*(-1) || parentBoard->getBox(bp.x() + x, bp.y() - x) == kind*(-1)*6) {
                    nPieces++;
                    xE = bp.x() + x;
                    yE = bp.y() - x;
                }
            }
            if (nPieces == 1) {
                emit this->takePiece(xE, yE, 1);
                chainJumper = true;
                return true;
            }
            else if (nPieces > 1) {
                return false;
            }
        }
        else { //left
            for (int x = -1; x > dx; x--){
                if (parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*(-1) || parentBoard->getBox(bp.x() + x, bp.y() + x) == kind*(-1)*6) {
                    nPieces++;
                    xE = bp.x() + x;
                    yE = bp.y() - x;
                }
            }
            if (nPieces == 1) {
                emit this->takePiece(xE, yE, 1);
                chainJumper = true;
                return true;
            }
            else if (nPieces > 1) {
                return false;
            }
        }
    }

    if (parentBoard->checkForJump(kind)) return false;                  //simple moves forbidden if jumps available
    return true;
}

void blackKing::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (getIfPlayPiece()) {

    this->setCursor(QCursor(Qt::ArrowCursor));
    QPoint newPos = this->getNewBPosition(this->pos());
    bool move = this->checkLegitMove(newPos);

    if (move) {                                                         //placing king in new legit position
        parentBoard->setBox(this->getBPosition(), 0);
        this->setBPosition(newPos);
        parentBoard->setBox(this->getBPosition(), this->kind*6);

        this->setWPosition();
    }

    this->setPos(this->getWPosition());

                                                                         //checking if chain of jumps should be started/terminated
    if (chainJumper && parentBoard->checkForKingJump(this->getBX()-1, this->getBY()-1, 1)){
        emit chainJump(this->getBX(), this->getBY(), 1);
    }
    else if (chainJumper && !parentBoard->checkForKingJump(this->getBX()-1, this->getBY()-1, 1)) {
        chainJumper = false;
        emit endChain(1);
    }
    else{
        if (move)
            emit nextMove();
        }
    }

    Q_UNUSED(event);
}
