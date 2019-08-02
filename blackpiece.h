#ifndef BLACKPIECE_H
#define BLACKPIECE_H

#include "abstractpiece.h"

class blackPiece : public abstractPiece
{
public:
    blackPiece(int i, QGraphicsItem *parent = nullptr);
    blackPiece(QPoint p, QGraphicsItem *parent = nullptr);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool checkLegitMove(QPoint pnt);
};

#endif // BLACKPIECE_H
