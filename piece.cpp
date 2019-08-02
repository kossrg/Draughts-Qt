#include "piece.h"


Piece::Piece(int i, bool col, QGraphicsItem *parent) : QGraphicsPixmapItem (parent)
{
    QString str;
    int x;
    int y;

    if (col){
        str = "D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_b.bmp";
        x = (i%4+1)*2-(1-(i/4)%2);
        y = i/4 + 1;
    }
    else {
        str = "D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_w.bmp";
        x = (i%4+1)*2-(i/4)%2;
        y = 8-i/4;
    }


    bPosition.setX(x);
    bPosition.setY(y);

    wPosition.setX(18 + (40 - WIDTH)/2 + SIZE * (bPosition.x() - 1));
    wPosition.setY(18 + (40 - HEIGHT)/2 + SIZE * (8 - bPosition.y()));


    QPixmap pix(str);
    pix.setMask(pix.createMaskFromColor(QColor(255, 255, 255)));
    setPixmap(pix);

    //setImage();

}

void Piece::setImage()
{
    QPixmap pix("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_b.bmp");
    //pix.setMask(QBitmap("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_m.bmp"));
    pix.setMask(pix.createMaskFromColor(QColor(255, 255, 255)));
    setPixmap(pix);
    //setPixmap(QPixmap("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_b.bmp"));

}

//void Piece::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
//    painter->drawPixmap(this->getX(), this->getY(), this->pixmap());
//    Q_UNUSED(option);
//    Q_UNUSED(widget);
//}

void Piece::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //setPixmap("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_w.bmp");

    this->captured = true;
    this->setCursor(QCursor(Qt::ClosedHandCursor));
    mouseOffset = this->pos() - mapToScene(event->pos());

}

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
   this->setPos(mapToScene(event->pos()) + mouseOffset);

}

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    this->captured = false;
    this->setCursor(QCursor(Qt::ArrowCursor));

    Q_UNUSED(event);
}

const QPointF Piece::getWPosition()
{
    return wPosition;
}
