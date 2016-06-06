#ifndef GAMEWONWINDOW_H
#define GAMEWONWINDOW_H

#include <QMainWindow>

namespace Ui {
class gamewonwindow;
}

class gamewonwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit gamewonwindow(QWidget *parent = 0);
    ~gamewonwindow();

public slots:
    void accept();

private:
    Ui::gamewonwindow *ui;
};

#endif // GAMEWONWINDOW_H
