#ifndef PIECE_H
#define PIECE_H

//#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QCursor>

class Piece: public QGraphicsPixmapItem
{
private:
    QPointF bPosition;  //Position on gameboard
    QPointF wPosition;  //Actual position in graphics scene

    const int WIDTH  = 34;
    const int HEIGHT = 16;
    const int SIZE   = 40;

    bool captured = false;
    //bool king;
    //int color;

    QPointF mouseOffset;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


public:
    Piece(int i, bool col, QGraphicsItem *parent = nullptr);

    //void paint(QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *widget = 0);
    void setImage();

    const QPointF getWPosition();

    //virtual void setBPosition();
    //virtual void setWPosition();

};

#endif // PIECE_H
