#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <gameboard.h>
#include <popupwindow.h>
#include <gamewonwindow.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



signals:


public slots:
    void easyGame();
    void mediumGame();
    void hardGame();

    void gameOver();

    void wonGame();
    void lostGame();


private:
    Ui::MainWindow *ui;
    gameBoard* board;
    popupwindow *newPopWindow;
    gamewonwindow *newWinWindow;
    Ui::gamewonwindow *winui;
    Ui::popupwindow *popui;
};

#endif // MAINWINDOW_H
