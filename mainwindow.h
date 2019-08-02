#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QActionGroup>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QFile>
#include <QDataStream>

#include "boardbox.h"
#include "abstractpiece.h"
#include "blackpiece.h"
#include "blackking.h"
#include "whitepiece.h"
#include "whiteking.h"
#include "gameboard.h"
#include "ai.h"
#include "move.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:          //all of that is from menu bar
    void About_D_triggered();
    void Rules_triggered();

    void New_triggered();
    void Save_triggered();
    void Load_triggered();
    void Quit_triggered();


    void PC_triggered();
    void Player_triggered();

    void ac0_triggered();
    void ac1_triggered();
    void ac2_triggered();
    void ac3_triggered();
    void ac4_triggered();
    void ac5_triggered();

public slots:
    void removePiece(int x, int y, int col);
    void replaceForKing(int x, int y, int col);
    void chainJump(int x, int y, int kind);
    void stopChain(int kind);
    void newMove();
    void WWin();
    void BWin();

private:
    Ui::MainWindow *ui;

    AI *adversary = nullptr;

    gameBoard *board = nullptr;

    abstractPiece *bPiece[12];
    abstractPiece *wPiece[12];

    BoardBox *box[8][8];

    QGraphicsTextItem *vMark[8];
    QGraphicsTextItem *hMark[8];

    QGraphicsScene *scene;
    QGraphicsView *view;

    bool vsPC = true;
    int pc_depth = 3;
    bool ongoing = false;

    void initialize();

    void PCMove();

    bool chkForWPieces();
    bool chkForBPieces();

        //everything beneath is for menu bar construction
    void createMenus();
    void createActions();
    void createAGroups();

    QMenu *mGame;
    QMenu *mOptions;
    QMenu *mAbout;

    QAction *acNew;
    QAction *acSave;
    QAction *acLoad;
    QAction *acQuit;

    QAction *acRules;
    QAction *acAbout;

    QMenu *omGameType;
    QMenu *omAIOptions;
    QMenu *omAIDepth;

    QActionGroup *omGmt;
    QAction *acVSPC;
    QAction *acVSPlayer;

    QActionGroup *omAi;
    QAction *acMiniTree;
    QAction *acAlphaBeta;

    QActionGroup *omAid;
    QAction *ac0;
    QAction *ac1;
    QAction *ac2;
    QAction *ac3;
    QAction *ac4;
    QAction *ac5;

};

#endif // MAINWINDOW_H
