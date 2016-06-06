#include "gamewonwindow.h"
#include "ui_gamewonwindow.h"

gamewonwindow::gamewonwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gamewonwindow)
{
    ui->setupUi(this);
}

/** press accept button to close window
 */
void gamewonwindow::accept() {
    this->close();
}

gamewonwindow::~gamewonwindow()
{
    delete ui;
}
