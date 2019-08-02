#include "piece2.h"

piece2::piece2(QWidget *parent) : QLabel (parent)
{

}

void piece2::mousePressEvent(QMouseEvent *event)
{
    //int x = event->x() - this->x();
    //int y = event->y() - this->y();

    //this->move(event->x() - this->x(), event->y() - this->y());
    this->captured = true;


    //this->x() = x;

    //this->y() = event->y();
    //if (event->type() == QEvent::MouseMove)
    //{
        //this->move(this->x() + event->x(), this->y()+event->y());
    //}
    Q_UNUSED(event);

}

void piece2::mouseReleaseEvent(QMouseEvent *event)
{
    this->captured = false;
    int x, y;
    if ( (this->y()-50)%40 < 20){
        y = 50 + 10 + 40*((this->y()-50)/40);
    }
    else{
        y = 50 + 10 + 40*((this->y()-60)/40) + 40;
    }
    if ((this->x()-15)%40 < 20){
        x = 15 + 3 + 40*((this->x()-15)/40);
    }
    else{
        x = 15 + 3 + 40*((this->x()-15)/40) + 40;
    }
    if (y < 50) y += 50;
    if (x < 15) x += 15;
    if (y > 370) y -=40;
    if (x > 335) x -=40;

    int newx = (x-15)/40 + 1;
    int newy = 8 - (y-50)/40;

    int incx = this->coordX - newx;
    int incy = this->coordY - newy;

    if (this->checkLegitMove(incx, incy)){
        this->coordX = newx;
        this->coordY = newy;
        this->move(x, y);
    }
    else {
        x = 15 + 3 + (this->coordX - 1)*40;
        y = 50 + 10 +(8 - this->coordY)*40;
        this->move(x, y);
    }

Q_UNUSED(event);

}

void piece2::mouseMoveEvent(QMouseEvent *event)
{
    static const int x = event->x();
    static const int y = event->y();

    if (this->captured){
        this->move(this->x() + event->x() - x, this->y() + event->y() - y);
    }
}

bool piece2::checkLegitMove(int x, int y)
{
    if (!(x != 1 xor x != -1)) return false;
    if (!(y != 1 xor y != -1)) return false;
    if ((this->coordX + x) > 9 || (this->coordX + x) < 0) return false;
    if ((this->coordY + y) > 9 || (this->coordY + y) < 0) return false;

    return true;
}

void piece2::place()
{
    this->setGeometry(15+4+40*(this->coordX - 1), 35+10+40*(8-this->coordY), 34, 20);

}

/*
    QPixmap pix1("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_b.bmp");
    pix1.setMask(pix1.createMaskFromColor(QColor(255, 255, 255)));

    for (int i = 0; i < 12; i++){
        bPiece[i] = new piece2(this);
        bPiece[i]->coordY = 8 - i/4;
        bPiece[i]->coordX = (i%4+1)*2-(i/4)%2;
        //bPiece[i]->setGeometry(18+40*(bPiece[i]->coordX - 1), 60+40*(8-bPiece[i]->coordY), 34, 20);
        bPiece[i]->place();
        bPiece[i]->setPixmap(pix1);


    }
    QPixmap pix2("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_w.bmp");
    pix2.setMask(pix2.createMaskFromColor(QColor(255, 255, 255)));
    for (int i = 0; i < 12; i++){
        wPiece[i] = new piece2(this);

        wPiece[i]->coordY = i/4 + 1;
        wPiece[i]->coordX = (i%4+1)*2-(1-(i/4)%2);
        wPiece[i]->place();
        //wPiece[i]->setGeometry(18+40*(wPiece[i]->coordX - 1), 260+40*(8-wPiece[i]->coordY), 34, 20);
        wPiece[i]->setPixmap(pix2);
    }

void abstractPiece::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    this->setCursor(QCursor(Qt::ArrowCursor));
    QPoint newPos = this->getNewBPosition(this->pos());
    bool move = this->checkLegitMove(newPos);

    if (move) {
        parentBoard->setBox(this->getBPosition(), 0);
        this->setBPosition(newPos);
        parentBoard->setBox(this->getBPosition(), this->kind);

        this->setWPosition();
    }

    this->setPos(this->getWPosition());
    if (this->getBY() == 1)
        emit makeKing(this->getBX(), 1, 0);
    if (this->getBY() == 8)
        emit makeKing(this->getBX(), 8, 1);



    Q_UNUSED(event);
}

bool abstractPiece::checkForJumps()
{
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2) && parentBoard->getBox(j, i) == 1*kind*6){
                 if (checkForKingJumps(j, i)) return true;
            }
            if (!((i+j)%2) && parentBoard->getBox(j, i) == 1*kind){
                if (j - 2 > 0 && i - 2*kind > 0 && i - 2*kind < 9 && parentBoard->getBox(j - 2, i - 2*kind) == 0 && (parentBoard->getBox(j - 1, i - 1*kind) == (-1)*kind || parentBoard->getBox(j - 1, i - 1*kind) == (-1)*kind*6))
                    return true;
                if (j + 2 < 9 && i - 2*kind > 0 && i - 2*kind < 9 && parentBoard->getBox(j + 2, i - 2*kind) == 0 && (parentBoard->getBox(j + 1, i - 1*kind) == (-1)*kind || parentBoard->getBox(j - 1, i - 1*kind) == (-1)*kind*6))
                    return true;
            }
        }
    }
    return false;
}

bool abstractPiece::checkForKingJumps(int x, int y)
{
    for(int i = 1, j = 1; x + j < 9 && y + i < 9; i++, j++){
        if (parentBoard->getBox(x+j, y+i) == kind || parentBoard->getBox(x+j, y+i) == kind*6) break;
        if (parentBoard->getBox(x+j, y+i) == kind*(-1) || parentBoard->getBox(x+j, y+i) == kind*6*(-1)){
            if (x+j+1 < 9 && y+i+1 < 9 && parentBoard->getBox(x+j+1, y+i+1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y + i < 9; i++, j++){
        if (parentBoard->getBox(x-j, y+i) == kind || parentBoard->getBox(x-j, y+i) == kind*6) break;
        if (parentBoard->getBox(x-j, y+i) == kind*(-1) || parentBoard->getBox(x-j, y+i) == kind*6*(-1)){
            if (x-j-1 > 0 && y+i+1 < 9 && parentBoard->getBox(x-j-1, y+i+1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x + j < 9 && y - i > 0; i++, j++){
        if (parentBoard->getBox(x+j, y-i) == kind || parentBoard->getBox(x+j, y-i) == kind*6) break;
        if (parentBoard->getBox(x+j, y-i) == kind*(-1) || parentBoard->getBox(x+j, y-i) == kind*6*(-1)){
            if (x+j+1 < 9 && y-i-1 > 0 && parentBoard->getBox(x+j+1, y-i-1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y - i > 0; i++, j++){
        if (parentBoard->getBox(x-j, y-i) == kind || parentBoard->getBox(x-j, y-i) == kind*6) break;
        if (parentBoard->getBox(x-j, y-i) == kind*(-1) || parentBoard->getBox(x-j, y-i) == kind*6*(-1)){
            if (x-j-1 > 0 && y-i-1 > 0 && parentBoard->getBox(x-j-1, y-i-1) == 0)
                return true;
        }
    }
    return false;
}

  FOR WHITE
        for (int i = 8; i > 0; i--){
            for (int j = 1; j < 9; j++){
                if (!(i+j)%2 && gBoard.getBox(j, i) == -1){
                    if (j - 1 > 0 && i + 1 < 9 && gBoard.getBox(j - 1, i + 1) == 0)
                        pMoves.push_back(new move(QPoint(j, i), QPoint(j-1, i+1), kind));
                    if (j + 1 < 9 && i + 1 < 9 && gBoard.getBox(j + 1, i + 1) == 0)
                        pMoves.push_back(new move(QPoint(j, i), QPoint(j+1, i+1), kind));
                }
            }
        }

void MainWindow::paintEvent(QPaintEvent *)
{

    QPainter paint1(this);



    paint1.setPen(Qt::NoPen);
    paint1.setBrush(Qt::SolidPattern);

    paint1.setBrush(QColor(255, 255, 255));
    paint1.drawRect(QRect(15, 50, 320, 320));

    paint1.setBrush(QColor(0, 151, 0));

    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if ((i+j)%2){
               paint1.drawRect(QRect((j%8)*40+15, (i%8)*40+50, 40, 40));
            }
        }
    }

    //QPixmap pix("D:\\Programming\\prg\\Educational tasks\\MenuBar\\res\\piece_b.bmp");


    //paint1.drawPixmap(pis->getX(), pis->getY(), pis->pixmap());
    //pis->paint(&paint1, NULL, this);



}

bool MainWindow::chkForMovJmp(int k)
{
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2) && board->getBox(j, i) == 1*k*6){
                if (chkForKMov(j, i, k)) return true;
            }
            if (!((i+j)%2) && board->getBox(j, i) == 1*k){
                if (j - 1 > 0 && i - 1*k > 0 && i - 1*k < 9 && board->getBox(j - 1, i - 1*k) == 0)
                    return true;
                if (j + 1 < 9 && i - 1*k > 0 && i - 1*k < 9 && board->getBox(j + 1, i - 1*k) == 0)
                    return true;
            }
        }
    }
    for (int i = 8; i > 0; i--){
        for (int j = 1; j < 9; j++){
            if (!((i+j)%2) && board->getBox(j, i) == 1*k*6){
                 if (chkForKJmp(j, i, k)) return true;
            }
            if (!((i+j)%2) && board->getBox(j, i) == 1*k){
                if (j - 2 > 0 && i - 2*k > 0 && i - 2*k < 9 && board->getBox(j - 2, i - 2*k) == 0 && (board->getBox(j - 1, i - 1*k) == (-1)*k || board->getBox(j - 1, i - 1*k) == (-1)*k*6))
                    return true;
                if (j + 2 < 9 && i - 2*k > 0 && i - 2*k < 9 && board->getBox(j + 2, i - 2*k) == 0 && (board->getBox(j + 1, i - 1*k) == (-1)*k || board->getBox(j - 1, i - 1*k) == (-1)*k*6))
                    return true;
            }
        }
    }
    return false;
}

bool MainWindow::chkForKMov(int x, int y, int k)
{
    if (x+1 < 9 && y+1 < 9 && board->getBox(x+1, y+1) == 0) return true;
    if (x-1 > 0 && y+1 < 9 && board->getBox(x-1, y+1) == 0) return true;
    if (x+1 < 9 && y-1 > 0 && board->getBox(x+1, y-1) == 0) return true;
    if (x-1 > 0 && y-1 > 0 && board->getBox(x-1, y-1) == 0) return true;

    return false;
}

bool MainWindow::chkForKJmp(int x, int y, int k)
{
    for(int i = 1, j = 1; x + j < 9 && y + i < 9; i++, j++){
        if (board->getBox(x+j, y+i) == k || board->getBox(x+j, y+i) == k*6) break;
        if (board->getBox(x+j, y+i) == k*(-1) || board->getBox(x+j, y+i) == k*6*(-1)){
            if (x+j+1 < 9 && y+i+1 < 9 && board->getBox(x+j+1, y+i+1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y + i < 9; i++, j++){
        if (board->getBox(x-j, y+i) == k || board->getBox(x-j, y+i) == k*6) break;
        if (board->getBox(x-j, y+i) == k*(-1) || board->getBox(x-j, y+i) == k*6*(-1)){
            if (x-j-1 > 0 && y+i+1 < 9 && board->getBox(x-j-1, y+i+1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x + j < 9 && y - i > 0; i++, j++){
        if (board->getBox(x+j, y-i) == k || board->getBox(x+j, y-i) == k*6) break;
        if (board->getBox(x+j, y-i) == k*(-1) || board->getBox(x+j, y-i) == k*6*(-1)){
            if (x+j+1 < 9 && y-i-1 > 0 && board->getBox(x+j+1, y-i-1) == 0)
                return true;
        }
    }

    for(int i = 1, j = 1; x - j > 0 && y - i > 0; i++, j++){
        if (board->getBox(x-j, y-i) == k || board->getBox(x-j, y-i) == k*6) break;
        if (board->getBox(x-j, y-i) == k*(-1) || board->getBox(x-j, y-i) == k*6*(-1)){
            if (x-j-1 > 0 && y-i-1 > 0 && board->getBox(x-j-1, y-i-1) == 0)
                return true;
        }
    }
    return false;
}

        //if (vsPC) {
            wPiece[i]->setPlayPiece(true);
        //}
        //else {
        //    wPiece[i]->setPlayPiece(false);
        //}
*/

