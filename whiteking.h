#ifndef WHITEKING_H
#define WHITEKING_H

#include "abstractpiece.h"

class whiteKing : public abstractPiece
{
public:
    whiteKing(QPoint p, QGraphicsItem *parent = nullptr);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool checkLegitMove(QPoint pnt);
};

#endif // WHITEKING_H
