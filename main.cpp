#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow WndMain;

    WndMain.show();

    return a.exec();
}
