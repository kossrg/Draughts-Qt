#ifndef BOARDBOX_H
#define BOARDBOX_H

#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>


class BoardBox : public QGraphicsRectItem  //class represents boxes on the game board
{
private:

    QPointF bPosition;  //Position on gameboard
    QPointF wPosition;  //Actual position in graphics scene

    const int SIZE = 40;

public:
    BoardBox(int x, int y, QGraphicsRectItem *parent = nullptr);

    const QPointF getWPosition();
};


#endif // BOARDBOX_H
