#include "popupwindow.h"
#include "ui_popupwindow.h"

popupwindow::popupwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::popupwindow)
{
    ui->setupUi(this);
}

/** press accept button to close window
 */
void popupwindow::accept() {
    this->close();
}

popupwindow::~popupwindow()
{
    delete ui;
}
