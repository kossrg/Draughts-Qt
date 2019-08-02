#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boardbox.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setFixedSize(350, 380);

    createActions();
    createMenus();
    createAGroups();

    adversary = new AI(1, 0, 3);

    scene = new QGraphicsScene(this);
    view = new QGraphicsView(this);
    view->setScene(scene);

    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(Qt::lightGray);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setGeometry(0, 20, 350, 380);
    view->setSceneRect(0, 0, 350, 360);
    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 349, 359);

    for (int i = 0; i < 12; i++){
        bPiece[i] = nullptr;
        wPiece[i] = nullptr;
    }

    initialize();


}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::initialize()
{
        //Drawing Coordinates
    char a = 'A';
    for (int i = 0; i < 8; i++){
        vMark[i] = new QGraphicsTextItem();
        vMark[i]->setDefaultTextColor(Qt::black);
        vMark[i]->setFont(QFont("MS Shell Dlg 2", 14));
        vMark[i]->setPlainText(QString::number(8 - i));
        vMark[i]->setPos(0, i*40 + 20);

        scene->addItem(vMark[i]);

        hMark[i] = new QGraphicsTextItem();
        hMark[i]->setDefaultTextColor(Qt::black);
        hMark[i]->setFont(QFont("MS Shell Dlg 2", 14));
        hMark[i]->setPlainText(QString(a + i));
        hMark[i]->setPos(i*40 + 29, 333);

        scene->addItem(hMark[i]);
    }

        //Drawing Board
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            box[i][j] = new BoardBox(j, i);
            box[i][j]->setPos(box[i][j]->getWPosition());
            scene->addItem(box[i][j]);
        }
    }
}

            /* GAME MECHANICS PART*/

void MainWindow::removePiece(int x, int y, int col)
{
    int i {0};
    if (col){
        for (i = 0; i < 12; i++){
            if (wPiece[i]->getBX() == x && wPiece[i]->getBY() == y) break;
        }
        wPiece[i]->setBPosition(0 , 0);
        scene->removeItem(wPiece[i]);
    }
    else {
        for (i = 0; i < 12; i++){
            if (bPiece[i]->getBX() == x && bPiece[i]->getBY() == y) break;
        }
        bPiece[i]->setBPosition(0 , 0);
        scene->removeItem(bPiece[i]);
    }

    board->setBox(x, y, 0);

}

void MainWindow::replaceForKing(int x, int y, int col)
{
    int i {0};
    if (col){
        for (i = 0; i < 12; i++){
            if (wPiece[i]->getBX() == x && wPiece[i]->getBY() == y) break;
        }
        scene->removeItem(wPiece[i]);
        delete wPiece[i];
        wPiece[i] = new whiteKing(QPoint(x, y));
        wPiece[i]->parentBoard = board;

            wPiece[i]->setPlayPiece(true);

        connect(wPiece[i], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
        connect(wPiece[i], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
        connect(wPiece[i], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
        connect(wPiece[i], SIGNAL(nextMove()), this, SLOT(newMove()));
        wPiece[i]->setPos(wPiece[i]->getWPosition());
        scene->addItem(wPiece[i]);
        if (i==0) i++;
        else i = 0;
        if (!wPiece[i]->getIfPlayPiece()) stopChain(-1);
        else {
            newMove();
        }

    }
    else {
        for (i = 0; i < 12; i++){
            if (bPiece[i]->getBX() == x && bPiece[i]->getBY() == y) break;
        }
        scene->removeItem(bPiece[i]);
        delete bPiece[i];
        bPiece[i] = new blackKing(QPoint(x, y));
        bPiece[i]->parentBoard = board;

            bPiece[i]->setPlayPiece(true);

        connect(bPiece[i], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
        connect(bPiece[i], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
        connect(bPiece[i], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
        connect(bPiece[i], SIGNAL(nextMove()), this, SLOT(newMove()));
        bPiece[i]->setPos(bPiece[i]->getWPosition());
        scene->addItem(bPiece[i]);
        if (i==0) i++;
        else i = 0;

        if (!bPiece[i]->getIfPlayPiece() && !vsPC)
            stopChain(1);
        else {
        if (!vsPC) newMove();
        }
    }

}

void MainWindow::chainJump(int x, int y, int k)
{
    int i {0};
    if (k == 1){
        for (i = 0; i < 12; i++){
            if (bPiece[i]->getBX() != x && bPiece[i]->getBY() != y)
                bPiece[i]->setPlayPiece(false);
        }
    }
    if (k == -1){
        for (i = 0; i < 12; i++){
            if (wPiece[i]->getBX() != x && wPiece[i]->getBY() != y)
                wPiece[i]->setPlayPiece(false);
        }
    }
}

void MainWindow::stopChain(int k)
{
    if (k == 1) {
        for (int i = 0; i < 12; i++){
            bPiece[i]->setPlayPiece(true);
        }
    }
    if (k == -1) {
        for (int i = 0; i < 12; i++){
            wPiece[i]->setPlayPiece(true);
        }
    }
    newMove();
}

void MainWindow::newMove()
{
    for (int i = 0; i < 12; i++){
        bPiece[i]->togglePlayPiece();
        wPiece[i]->togglePlayPiece();
    }

    if (bPiece[0]->getIfPlayPiece()){
        if (!chkForBPieces() || !board->checkForMoveJump(1)){
            ongoing = false;
            for (int i = 0; i < 12; i++){
                bPiece[i]->togglePlayPiece();
            }
            WWin();
        }
    }
    if (wPiece[0]->getIfPlayPiece()){
        if (!chkForWPieces() || !board->checkForMoveJump(-1)){
            ongoing = false;
            for (int i = 0; i < 12; i++){
                wPiece[i]->togglePlayPiece();
            }
            BWin();
        }
    }

    adversary->gBoard = *board;
    if (vsPC && ongoing) PCMove();
}

void MainWindow::PCMove()
{
    dMove mv;
    if (adversary->depth == 0)
        mv = adversary->getRandMove();
    else
        mv = adversary->getDecision();

    if (mv.kind){           //mv.kind = 0  -  no valid moves
        int i {0};
        for (i = 0; i < 12; i++){
            if (bPiece[i]->getBX() == mv.source.x() && bPiece[i]->getBY() == mv.source.y()) break;
        }
        bPiece[i]->setBPosition(mv.destination);
        bPiece[i]->setWPosition();
        bPiece[i]->setPos(bPiece[i]->getWPosition());

        if (mv.jump){       //jumping piece
            for (i = 0; i < 12; i++){
                if (wPiece[i]->getBX() == mv.target.x() && wPiece[i]->getBY() == mv.target.y()) break;
            }
            scene->removeItem(wPiece[i]);
            wPiece[i]->setBPosition(0 , 0);
        }

        board->makeMove(&mv);
        adversary->gBoard = *board;

            //king chain jumps
        if (mv.jump && board->getBox(mv.destination) == 6 && board->checkForKingJump(mv.destination.x()-1, mv.destination.y()-1, 1)){
            dMove mvc = mv;
            while (true){
                mvc = adversary->getKingChainJump(mvc.destination.x(), mvc.destination.y());
                if (!mvc.kind) break;
                for (i = 0; i < 12; i++){
                    if (bPiece[i]->getBX() == mvc.source.x() && bPiece[i]->getBY() == mvc.source.y()) break;
                }
                bPiece[i]->setBPosition(mvc.destination);
                bPiece[i]->setWPosition();
                bPiece[i]->setPos(bPiece[i]->getWPosition());

                for (i = 0; i < 12; i++){
                    if (wPiece[i]->getBX() == mvc.target.x() && wPiece[i]->getBY() == mvc.target.y()) break;
                }
                scene->removeItem(wPiece[i]);
                wPiece[i]->setBPosition(0 , 0);
                board->makeMove(&mvc);

                adversary->gBoard = *board;
            }
        }

            //promoting piece to king
        if (mv.destination.y() == 1 && board->getBox(mv.destination) == 1){
            replaceForKing(mv.destination.x(), mv.destination.y(), 0);
            board->setBox(mv.destination, 6);
        }

            //piece chain jumps
        if(mv.jump && board->getBox(mv.destination) == 1 && board->checkForChainJump(mv.destination.x(), mv.destination.y(), 1)){
            dMove mvc = mv;
            while (true) {
                mvc = adversary->getChainJump(mvc.destination.x(), mvc.destination.y());
                if (!mvc.kind) break;
                for (i = 0; i < 12; i++){
                    if (bPiece[i]->getBX() == mvc.source.x() && bPiece[i]->getBY() == mvc.source.y()) break;
                }
                bPiece[i]->setBPosition(mvc.destination);
                bPiece[i]->setWPosition();
                bPiece[i]->setPos(bPiece[i]->getWPosition());

                for (i = 0; i < 12; i++){
                    if (wPiece[i]->getBX() == mvc.target.x() && wPiece[i]->getBY() == mvc.target.y()) break;
                }
                scene->removeItem(wPiece[i]);
                wPiece[i]->setBPosition(0 , 0);
                board->makeMove(&mvc);
                if (mvc.destination.y() == 1 && board->getBox(mvc.destination) == 1){
                    replaceForKing(mvc.destination.x(), mvc.destination.y(), 0);
                    board->setBox(mvc.destination, 6);
                    break;   // ???
                }
                adversary->gBoard = *board;
            }

        }

        for (int i = 0; i < 12; i++){
            bPiece[i]->togglePlayPiece();
            wPiece[i]->togglePlayPiece();
        }
        if (wPiece[0]->getIfPlayPiece()){
            if (!chkForWPieces() || !board->checkForMoveJump(-1)){
                ongoing = false;
                for (int i = 0; i < 12; i++){
                    wPiece[i]->togglePlayPiece();
                }
                BWin();
            }
        }
    }
    else{
        WWin();
    }

}

bool MainWindow::chkForWPieces()
{
    for (int i = 0; i < 12; i++){
        if (wPiece[i]->getBX() > 0) return true;
    }
    return false;
}

bool MainWindow::chkForBPieces()
{
    for (int i = 0; i < 12; i++){
        if (bPiece[i]->getBX() > 0) return true;
    }
    return false;
}

void MainWindow::WWin()
{
     QMessageBox::information(this, "The Game", "WHITE WIN!!!");
}

void MainWindow::BWin()
{
     QMessageBox::information(this, "The Game", "BLACK WIN!!!");
}

            /*MENU BAR SLOTS*/

void MainWindow::New_triggered()
{
    ongoing = true;

    if (board != nullptr) delete board;

    scene->clear();
    initialize();

    board = new gameBoard();
    board->initialize();

    adversary->gBoard = *board;

        //Setting initial disposition
    for (int i = 0; i < 12; i++){
        bPiece[i] = new blackPiece(i);
        bPiece[i]->parentBoard = board;
        connect(bPiece[i], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
        connect(bPiece[i], SIGNAL(makeKing(int, int, int)), this, SLOT(replaceForKing(int, int, int)));
        connect(bPiece[i], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
        connect(bPiece[i], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
        connect(bPiece[i], SIGNAL(nextMove()), this, SLOT(newMove()));
        bPiece[i]->setPos(bPiece[i]->getWPosition());
        scene->addItem(bPiece[i]);
    }
    for (int i = 0; i < 12; i++){
        wPiece[i] = new whitePiece(i);
        wPiece[i]->setPlayPiece(true);
        wPiece[i]->parentBoard = board;
        connect(wPiece[i], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
        connect(wPiece[i], SIGNAL(makeKing(int, int, int)), this, SLOT(replaceForKing(int, int, int)));
        connect(wPiece[i], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
        connect(wPiece[i], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
        connect(wPiece[i], SIGNAL(nextMove()), this, SLOT(newMove()));
        wPiece[i]->setPos(wPiece[i]->getWPosition());
        scene->addItem(wPiece[i]);
    }

    omGameType->setEnabled(true);

}

void MainWindow::Save_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save game", "/draughts", "Text files (*.txt)");
    //QMessageBox::information(this, "The Game", fileName);
    QFile sFile(fileName);
    if (sFile.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&sFile);
        for (int y = 8; y > 0; y--){
            for (int x = 1; x < 9; x++){
                stream << board->getBox(x, y);
            }
        }

        if (bPiece[0]->getIfPlayPiece()) stream << 3;
        if (wPiece[0]->getIfPlayPiece()) stream << 5;

        sFile.close();
        if (stream.status() != QDataStream::Ok)
            QMessageBox::warning(this, "The Game", "Write error!");
    }
}

void MainWindow::Load_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load game", "/draughts");
    QFile lFile(fileName);
    //QString str;                  //for debug
    short val;
    if (lFile.open(QIODevice::ReadOnly))
    {
        QDataStream stream(&lFile);


        delete board;
        board = new gameBoard();

        int ib {0};
        int iw {0};

        scene->clear();
        initialize();

            //loading values and setting new game
        for (int y = 8; y > 0; y--){
            for (int x = 1; x < 9; x++){
                stream >> val;
                //str += QString::number(val);          //for debug
                board->setBox(x, y, val);
                if (val == 1){
                    bPiece[ib] = new blackPiece(QPoint(x, y));
                    bPiece[ib]->parentBoard = board;
                    connect(bPiece[ib], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
                    connect(bPiece[ib], SIGNAL(makeKing(int, int, int)), this, SLOT(replaceForKing(int, int, int)));
                    connect(bPiece[ib], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
                    connect(bPiece[ib], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
                    connect(bPiece[ib], SIGNAL(nextMove()), this, SLOT(newMove()));
                    bPiece[ib]->setPos(bPiece[ib]->getWPosition());
                    scene->addItem(bPiece[ib]);
                    ib++;
                }
                if (val == 6){
                    bPiece[ib] = new blackKing(QPoint(x, y));
                    bPiece[ib]->parentBoard = board;
                    connect(bPiece[ib], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
                    connect(bPiece[ib], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
                    connect(bPiece[ib], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
                    connect(bPiece[ib], SIGNAL(nextMove()), this, SLOT(newMove()));
                    bPiece[ib]->setPos(bPiece[ib]->getWPosition());
                    scene->addItem(bPiece[ib]);
                    ib++;
                }
                if (val == -1){
                    wPiece[iw] = new whitePiece(QPoint(x, y));
                    wPiece[iw]->parentBoard = board;
                    connect(wPiece[iw], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
                    connect(wPiece[iw], SIGNAL(makeKing(int, int, int)), this, SLOT(replaceForKing(int, int, int)));
                    connect(wPiece[iw], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
                    connect(wPiece[iw], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
                    connect(wPiece[iw], SIGNAL(nextMove()), this, SLOT(newMove()));
                    wPiece[iw]->setPos(wPiece[iw]->getWPosition());
                    scene->addItem(wPiece[iw]);
                    iw++;
                }
                if (val == -6){
                    wPiece[iw] = new whiteKing(QPoint(x, y));
                    wPiece[iw]->parentBoard = board;
                    //if (vsPC) wPiece[i]->setPlayPiece(true);
                    connect(wPiece[iw], SIGNAL(takePiece(int, int, int)), this, SLOT(removePiece(int, int, int)));
                    connect(wPiece[iw], SIGNAL(chainJump(int, int, int)), this, SLOT(chainJump(int, int, int)));
                    connect(wPiece[iw], SIGNAL(endChain(int)), this, SLOT(stopChain(int)));
                    connect(wPiece[iw], SIGNAL(nextMove()), this, SLOT(newMove()));
                    wPiece[iw]->setPos(wPiece[iw]->getWPosition());
                    scene->addItem(wPiece[iw]);
                    iw++;
                }
            }
        }

        //establishing moves order
        stream >> val;

        if (val == 3){
            for (int i = 0; i < ib; i++){
                bPiece[i]->setPlayPiece(true);
            }
            for (int i = 0; i < iw; i++){
                wPiece[i]->setPlayPiece(false);
            }
        }
        if (val == 5){
            for (int i = 0; i < ib; i++){
                bPiece[i]->setPlayPiece(false);
            }
            for (int i = 0; i < iw; i++){
                wPiece[i]->setPlayPiece(true);
            }
        }

        //unused pieces should exist
        for (int i = ib; i < 12; i++){
            bPiece[i] = new blackPiece(i);
            bPiece[i]->parentBoard = board;
            bPiece[i]->setBPosition(0, 0);
            bPiece[i]->setWPosition();
        }
        for (int i = iw; i < 12; i++){
            wPiece[i] = new whitePiece(i);
            wPiece[i]->parentBoard = board;
            wPiece[i]->setBPosition(0, 0);
            wPiece[i]->setWPosition();
        }

        adversary->gBoard = *board;
        omGameType->setEnabled(true);

        if (stream.status() != QDataStream::Ok)
            QMessageBox::warning(this, "The Game", "Load error!");
        lFile.close();
    }
    //QMessageBox::information(this, "The Game", str);      //for debug

    //if PC moves first...
    ongoing = true;
    if (acVSPC->isChecked()){
        vsPC = true;
    }
    else {
        vsPC = false;
    }
    if (acVSPC->isChecked() && val == 3) PCMove();

}

void MainWindow::Quit_triggered()
{
    //Checks and Message "Save current game?" needs to be added
    QApplication::quit();
}

void MainWindow::About_D_triggered()
{
    QMessageBox::information(this, "Draughts", "Draughts 1.2\n 2019");
}

void MainWindow::Rules_triggered()
{
    QString str = "Pieces move forward diagonally to an adjacent unoccupied square. \n"
                  "If a player\'s piece moves into the last (\"kings\") row on the opposing player\'s side of the board, that piece becomes a \"king\". "
                  "It gains the ability to move back or forward and choose on which free square at this diagonal to stop. \n"
                  "An opponent\'s piece could be captured by \"jumping\" on vacant square immediately beyond it.\n"
                  "Jumping can be done forward and backward. Multiple-jump moves are possible if, when the jumping piece lands, there is another piece that can be jumped.\n"
                  "Jumping is mandatory and cannot be passed up to make a non-jumping move. However, one must make all the captures in that sequence.\n"
                  "Kings could jump on any vacant square besides a piece being captured.";
    QMessageBox::information(this, "Rules", str);
}

void MainWindow::PC_triggered()
{
    vsPC = true;
    if (bPiece[1]->getIfPlayPiece()) PCMove();
}

void MainWindow::Player_triggered()
{
    vsPC = false;
}

void MainWindow::ac0_triggered()
{
    adversary->depth = 0;
}

void MainWindow::ac1_triggered()
{
    adversary->depth = 1;
}

void MainWindow::ac2_triggered()
{
    adversary->depth = 2;
}

void MainWindow::ac3_triggered()
{
    adversary->depth = 3;
}

void MainWindow::ac4_triggered()
{
    adversary->depth = 4;
}

void MainWindow::ac5_triggered()
{
    adversary->depth = 5;
}


            /*CREATING MENU BAR*/

void MainWindow::createMenus()
{
    mGame = menuBar()->addMenu("&Game");
    mGame->addAction(acNew);
    mGame->addSeparator();
    mGame->addAction(acSave);
    mGame->addAction(acLoad);
    mGame->addSeparator();
    mGame->addAction(acQuit);

    mOptions = menuBar()->addMenu("O&ptions");
    omGameType = mOptions->addMenu("&Game Type");
    omGameType->addAction(acVSPC);
    omGameType->addAction(acVSPlayer);
    omGameType->setEnabled(false);
    mOptions->addSeparator();
    omAIOptions = mOptions->addMenu("AI options");
    omAIOptions->addAction(acMiniTree);
    omAIOptions->addAction(acAlphaBeta);
    omAIDepth = mOptions->addMenu("AI Depth");
    omAIDepth->addAction(ac0);
    omAIDepth->addAction(ac1);
    omAIDepth->addAction(ac2);
    omAIDepth->addAction(ac3);
    omAIDepth->addAction(ac4);
    omAIDepth->addAction(ac5);

    mAbout = menuBar()->addMenu("&About");
    mAbout->addAction(acRules);
    mAbout->addAction(acAbout);
}

void MainWindow::createActions()
{
    //Game menu
    acNew = new QAction("&New", this);
    acNew->setStatusTip("Start new game");
    connect(acNew, SIGNAL(triggered()), this, SLOT(New_triggered()));

    acSave = new QAction("&Save", this);
    acSave->setStatusTip("Save current game");
    connect(acSave, SIGNAL(triggered()), this, SLOT(Save_triggered()));

    acLoad = new QAction("&Load", this);
    acLoad->setStatusTip("Load saved game");
    connect(acLoad, SIGNAL(triggered()), this, SLOT(Load_triggered()));

    acQuit = new QAction("&Quit", this);
    acQuit->setStatusTip("Start new game");
    connect(acQuit, SIGNAL(triggered()), this, SLOT(Quit_triggered()));

    //Options menu
    acVSPC = new QAction("&vs. PC", this);
    acVSPC->setStatusTip("Play vs. PC adversary");
    acVSPC->setCheckable(true);
    acVSPC->setChecked(true);
    connect(acVSPC, SIGNAL(triggered()), this, SLOT(PC_triggered()));
    acVSPlayer = new QAction("&vs. Player", this);
    acVSPlayer->setStatusTip("Play vs. Human adversary");
    acVSPlayer->setCheckable(true);
    connect(acVSPlayer, SIGNAL(triggered()), this, SLOT(Player_triggered()));

    acMiniTree = new QAction("Minimax Tree");
    acMiniTree->setCheckable(true);
    //connect...
    acAlphaBeta = new QAction("Alpha-Beta Pruning");
    acAlphaBeta->setCheckable(true);
    acAlphaBeta->setEnabled(false);
    //connect...

    ac0 = new QAction("0", this);
    ac0->setCheckable(true);
    ac0->setStatusTip("Random moves");
    connect(ac0, SIGNAL(triggered()), this, SLOT(ac0_triggered()));
    ac1 = new QAction("1", this);
    ac1->setCheckable(true);
    connect(ac1, SIGNAL(triggered()), this, SLOT(ac1_triggered()));
    ac2 = new QAction("2", this);
    ac2->setCheckable(true);
    connect(ac2, SIGNAL(triggered()), this, SLOT(ac2_triggered()));
    ac3 = new QAction("3", this);
    ac3->setCheckable(true);
    ac3->setChecked(true);
    connect(ac3, SIGNAL(triggered()), this, SLOT(ac3_triggered()));
    ac4 = new QAction("4", this);
    ac4->setCheckable(true);
    connect(ac4, SIGNAL(triggered()), this, SLOT(ac4_triggered()));
    ac5 = new QAction("5", this);
    ac5->setCheckable(true);
    connect(ac5, SIGNAL(triggered()), this, SLOT(ac5_triggered()));

    //About menu
    acRules = new QAction("&Rules", this);
    acRules->setStatusTip("Rules of the game");
    connect(acRules, SIGNAL(triggered()), this, SLOT(Rules_triggered()));

    acAbout = new QAction("&About Droughts 1.2");
    acAbout->setStatusTip("About the game");
    connect(acAbout, SIGNAL(triggered()), this, SLOT(About_D_triggered()));


}

void MainWindow::createAGroups()
{
    omGmt = new QActionGroup(this);
    omGmt->setExclusive(true);
    omGmt->addAction(acVSPC);
    omGmt->addAction(acVSPlayer);

    omAi = new QActionGroup(this);
    omAi->setExclusive(true);
    omAi->addAction(acMiniTree);
    omAi->addAction(acAlphaBeta);
    acMiniTree->setChecked(true);

   omAid = new  QActionGroup(this);
   omAi->setExclusive(true);
   omAi->addAction(ac0);
   omAi->addAction(ac1);
   omAi->addAction(ac2);
   omAi->addAction(ac3);
   omAi->addAction(ac4);
   omAi->addAction(ac5);

}



