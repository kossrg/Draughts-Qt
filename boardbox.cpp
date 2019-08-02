#include "boardbox.h"

BoardBox::BoardBox(int x, int y, QGraphicsRectItem *parent):QGraphicsRectItem (parent)
{
    bPosition.setX(x+1);
    bPosition.setY(8-y);

    wPosition.setX(18 + SIZE * (bPosition.x() - 1));
    wPosition.setY(18 + SIZE * (8 - bPosition.y()));

    setRect(0, 0, SIZE, SIZE);

    QColor clr;

    if ((x + y)%2)
        clr = QColor(0, 151, 0);
    else
        clr = QColor(255, 255, 255);

    setPen(QPen(Qt::NoPen));
    setBrush(QBrush(clr, Qt::SolidPattern));
}

const QPointF BoardBox::getWPosition()
{
    return wPosition;
}

