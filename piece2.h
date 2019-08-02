#ifndef PIECE2_H
#define PIECE2_H
#include <QtWidgets>

class piece2:public QLabel
{
public:
    piece2(QWidget* parent =0);

    bool checkLegitMove(int x, int y);
    void place();

    bool captured = false;

    int coordX;
    int coordY;

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);


};

#endif // PIECE2_H
