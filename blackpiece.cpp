#include "blackpiece.h"


blackPiece::blackPiece(int i, QGraphicsItem *parent) : abstractPiece (parent)
{
    setBPosition((i%4+1)*2-(i/4)%2, 8-i/4);
    setWPosition();

    QPixmap pix(":/res/piece_b.png");
    pix.setMask(pix.createMaskFromColor(QColor(255, 255, 255)));
    setPixmap(pix);

    kind = 1;

}

blackPiece::blackPiece(QPoint p, QGraphicsItem *parent) : abstractPiece (parent)
{
    setBPosition(p.x(), p.y());
    setWPosition();

    QPixmap pix(":/res/piece_b.png");
    pix.setMask(pix.createMaskFromColor(QColor(255, 255, 255)));
    setPixmap(pix);

    kind = 1;
}

bool blackPiece::checkLegitMove(QPoint pnt)
{
    if (pnt.x() < 1 || pnt.x() > 8) return false;
    if (pnt.y() < 1 || pnt.y() > 8) return false;

    if (parentBoard->getBox(pnt.x(), pnt.y())) return false;

    QPoint bp = this->getBPosition();

    int dy = pnt.y() - bp.y();
    int dx = pnt.x() - bp.x();

    if (dy == -1 && (dx == 1 || dx == -1) && parentBoard->checkForJump(kind)) return false;         //simple moves forbidden if jumps available

    if (dx == 0) return false;

    //check if it's jump
    if (dy == -2 && dx == 2) {
        if (parentBoard->getBox(pnt.x()-1, pnt.y()+1) != kind*(-1) && parentBoard->getBox(pnt.x()-1, pnt.y()+1) != kind*(-1)*6) {
            return false;
        }
        else {
            emit this->takePiece(pnt.x()-1, pnt.y()+1, 1);
            chainJumper = true;
            return true;
        }
    }
    if (dy == -2 && dx == -2) {
        if (parentBoard->getBox(pnt.x()+1, pnt.y()+1) != kind*(-1) && parentBoard->getBox(pnt.x()+1, pnt.y()+1) != kind*(-1)*6) {
            return false;
        }
        else {
            emit this->takePiece(pnt.x()+1, pnt.y()+1, 1);
            chainJumper = true;
            return true;
        }
    }
    if (dy == 2 && dx == 2) {
        if (parentBoard->getBox(pnt.x()-1, pnt.y()-1) != kind*(-1) && parentBoard->getBox(pnt.x()-1, pnt.y()-1) != kind*(-1)*6) {
            return false;
        }
        else {
            emit this->takePiece(pnt.x()-1, pnt.y()-1, 1);
            chainJumper = true;
            return true;
        }

    }
    if (dy == 2 && dx == -2) {
        if (parentBoard->getBox(pnt.x()+1, pnt.y()-1) != kind*(-1) && parentBoard->getBox(pnt.x()+1, pnt.y()-1) != kind*(-1)*6) {
            return false;
        }
        else {
            emit this->takePiece(pnt.x()+1, pnt.y()-1, 1);
            chainJumper = true;
            return true;
        }
    }

    if (dy < -2 || dy > -1) return false;
    return true;
}

void blackPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (getIfPlayPiece()) {
    this->setCursor(QCursor(Qt::ArrowCursor));
    QPoint newPos = this->getNewBPosition(this->pos());
    bool move = this->checkLegitMove(newPos);

    if (move) {                                                     //placing piece in new legit position
        parentBoard->setBox(this->getBPosition(), 0);
        this->setBPosition(newPos);
        parentBoard->setBox(this->getBPosition(), this->kind);

        this->setWPosition();
    }

    this->setPos(this->getWPosition());

    if (this->getBY() == 1){                                        //checking if piece needs promotion to king or if chain of jumps should be started/terminated
        parentBoard->setBox(this->getBPosition(), 6);
        emit makeKing(this->getBX(), 1, 0);
    }
    else if (chainJumper && parentBoard->checkForChainJump(this->getBX(), this->getBY(), 1)){
        emit chainJump(this->getBX(), this->getBY(), 1);
    }
    else if ((chainJumper && !parentBoard->checkForChainJump(this->getBX(), this->getBY(), 1))){
        chainJumper = false;
        emit endChain(1);
    }
    else {
        if (move)
            emit nextMove();
    }

    }

    Q_UNUSED(event);
}
