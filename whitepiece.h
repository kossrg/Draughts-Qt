#ifndef WHITEPIECE_H
#define WHITEPIECE_H

#include "abstractpiece.h"

class whitePiece : public abstractPiece
{
public:
    whitePiece(int i, QGraphicsItem *parent = nullptr);
    whitePiece(QPoint p, QGraphicsItem *parent = nullptr);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    bool checkLegitMove(QPoint pnt);
};

#endif // WHITEPIECE_H
