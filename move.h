#ifndef MOVE_H
#define MOVE_H

#include <QPoint>


class dMove   //class represents moves by pieces
{
public:
    dMove();
    dMove(QPoint s, QPoint d, int k, bool j = false, QPoint t = QPoint(0, 0),  bool kt = false);
    dMove(dMove *m);

    int dx();
    int dy();

    QPoint source;
    QPoint destination;
    QPoint target;     //coordinates of piece being taken if any

    int kind;

    bool jump;

    bool kingTaken;
};

#endif // MOVE_H
