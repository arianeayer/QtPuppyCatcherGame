#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"
#include "popupwindow.h"
#include "ui_gamewonwindow.h"
#include "ui_popupwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/**
 * set board + game to easy
 */
void MainWindow::easyGame() {
    board = new gameBoard(this,10,100);
    this->setCentralWidget(board);
}

/**
 * set board + game to medium
 */
void MainWindow::mediumGame() {
    board = new gameBoard(this,15,50);
    this->setCentralWidget(board);
}

/**
 * set board + game to hard
 */
void MainWindow::hardGame() {
    board = new gameBoard(this,20,10);
    this->setCentralWidget(board);
}

/**
 * end current game
 */
void MainWindow::gameOver() {
    // Take the current board game out of the central widget of MainWindow
    QWidget* wid = this->centralWidget();
    wid->setParent(nullptr);
    // Line above will delete the widget gracefully (no memory errors)
    ui->setupUi(this);
    //lostGame();
}

/**
 * pop up when game/level is won
 */
void MainWindow::wonGame() {
   newWinWindow = new gamewonwindow;
   newWinWindow->show();
}

/**
 * pop up when game is lost
 */
void MainWindow::lostGame() {
    newPopWindow = new popupwindow;
    newPopWindow->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}
