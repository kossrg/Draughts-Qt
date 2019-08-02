#ifndef ABSTRACTPIECE_H
#define ABSTRACTPIECE_H

//#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPixmap>
#include <QBitmap>
#include <QCursor>
#include "gameboard.h"

class abstractPiece : public QObject, public QGraphicsPixmapItem        //basic class for template method
{
    Q_OBJECT
private:
    QPoint bPosition;               //Position on gameboard
    QPointF wPosition;              //Actual position in graphics scene

    const int WIDTH  = 34;
    const int HEIGHT = 16;
    const int SIZE   = 40;

    bool playPiece = false;

    QPointF mouseOffset;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


public:
    abstractPiece(QGraphicsItem *parent = nullptr);

    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void setBPosition(int x, int y);
    void setBPosition(QPoint pnt);

    int getBX();
    int getBY();

    void setWPosition();

    const QPoint getBPosition();
    const QPointF getWPosition();

    QPoint getNewBPosition(QPointF pnt);

    virtual bool checkLegitMove(QPoint pnt);

    void togglePlayPiece();
    void setPlayPiece(bool p);
    bool getIfPlayPiece();

    gameBoard *parentBoard;
    int kind;                               //determines color: 1 = black / -1 = white
    bool chainJumper = false;

signals:
    void takePiece(int, int, int);
    void makeKing(int, int, int);
    void nextMove();
    void chainJump(int, int, int);
    void endChain(int);
};

#endif // ABSTRACTPIECE_H
