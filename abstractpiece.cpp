#include "abstractpiece.h"

abstractPiece::abstractPiece(QGraphicsItem *parent) : QObject(nullptr), QGraphicsPixmapItem (parent)
{

}

void abstractPiece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //setPixmap("..\\res\\highlited_piece_.png");
    if (playPiece) {
        this->setCursor(QCursor(Qt::ClosedHandCursor));
        mouseOffset = this->pos() - mapToScene(event->pos());
    }
}

void abstractPiece::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (playPiece) {
    this->setPos(mapToScene(event->pos()) + mouseOffset);
    }
}

void abstractPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void abstractPiece::setBPosition(int x, int y)
{
    bPosition.setX(x);
    bPosition.setY(y);
}

void abstractPiece::setBPosition(QPoint pnt)
{
    this->bPosition = pnt;
}

void abstractPiece::setWPosition()
{
    wPosition.setX(18 + (40 - WIDTH)/2 + SIZE * (bPosition.x() - 1));
    wPosition.setY(18 + (40 - HEIGHT)/2 + SIZE * (8 - bPosition.y()));
}

const QPoint abstractPiece::getBPosition()
{
    return bPosition;
}

const QPointF abstractPiece::getWPosition()
{
    return wPosition;
}

QPoint abstractPiece::getNewBPosition(QPointF pnt)
{
    QPoint res;
    res.setX(int((pnt.x()) - 18)/SIZE + 1);
    res.setY(8 - (int((pnt.y()) - 18)/SIZE));
    return res;
}

bool abstractPiece::checkLegitMove(QPoint pnt)
{
        //redefined in child classes
}

void abstractPiece::togglePlayPiece()
{
    if (playPiece)
        playPiece = false;
    else
        playPiece = true;
}

void abstractPiece::setPlayPiece(bool p)
{
    playPiece = p;
}

bool abstractPiece::getIfPlayPiece()
{
    return playPiece;
}

int abstractPiece::getBX()
{
    return bPosition.x();
}

int abstractPiece::getBY()
{
    return bPosition.y();
}
