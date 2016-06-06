#ifndef POPUPWINDOW_H
#define POPUPWINDOW_H

#include <QMainWindow>
#include <gameboard.h>


namespace Ui {
class popupwindow;
}

class popupwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit popupwindow(QWidget *parent = 0);
    ~popupwindow();

public slots:
    void accept();

private:
    Ui::popupwindow *ui;
};

#endif // POPUPWINDOW_H
