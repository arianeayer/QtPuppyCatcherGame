#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <random>
#include <chrono>
#include <QTimer>
#include <QVector>

#include <iostream>


namespace Ui {
class gameBoard;
}

class gameBoard: public QWidget
{
    Q_OBJECT

public slots:
    void updateDogs();
    //void updateCars();

signals:
    void gameOver();
    void lostGame();
    void wonGame();
    void mediumGame();
    void hardGame();
    void returnMenu();

public:
    explicit gameBoard(QWidget *parent=0, size_t board_size = 10, size_t dog_speed = 3);
    ~gameBoard();

    void keyPressEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *e);

    bool catchDog();
    bool catchDog(int x, int y);
    void updateAvatar(int px, int py, int nx, int ny);
    void updateAfterCatch(bool flag);
    bool isValidCatch(int x, int y);

    int gametype_indicator;  // 1 = easy, 2 = medium, 3 = hard

private:
    Ui::gameBoard *ui;

    //Images
    const QPixmap* avatar_image;
    QPixmap* dog_image;
    QPixmap* grass_image;
    QPixmap* car_image;
    QPixmap* asphalt_image;
    QPixmap* tree_image;

    //Moving parts
    QPoint *avatar_position;
    QPoint *dog_positions;
    QPoint *car_positions;
    QPoint *tree_positions;

    size_t number_dog;
    size_t number_cars;
    size_t number_trees;
    std::vector<int> yvals;

    std::vector<int> treex;
    std::vector<int> treey;

    // Top bar variables
    QWidget* Top;
    unsigned int life_count;
    QLabel** life_counter;
    unsigned int capture_count;
    QLabel** capture_counter;


    // Board Variables
    QWidget *Board;
    size_t board_size;
    size_t dog_speed;
    //size_t car_speed;
    QLabel** labels;

};

#endif // GAMEBOARD_H
