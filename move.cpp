#include "move.h"

dMove::dMove()
{

}

dMove::dMove(QPoint s, QPoint d, int k, bool j, QPoint t,  bool kt)
{
    source = s;
    destination = d;
    kind = k;
    jump = j;
    target = t;
    kingTaken = kt;
}

dMove::dMove(dMove *m)
{
    source = m->source;
    destination = m->destination;
    kind = m->kind;
    jump = m->jump;
    target = m->target;
    kingTaken = m->kingTaken;
}

int dMove::dx()
{
    return destination.x() - source.x();
}

int dMove::dy()
{
    return destination.y() - source.y();
}
