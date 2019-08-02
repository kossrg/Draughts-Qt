#ifndef BLACKKING_H
#define BLACKKING_H

#include "abstractpiece.h"

class blackKing : public abstractPiece
{
public:
    blackKing(QPoint p, QGraphicsItem *parent = nullptr);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool checkLegitMove(QPoint pnt);
};

#endif // BLACKKING_H
