#include "efidaq.h"
#include "ui_efidaq.h"
#include "testsetup.h"

EFIDAQ::EFIDAQ(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EFIDAQ)
{
    // Initializes the window with the correct ".ui" file for the
    // window. If this setupUi function is not called like this,
    // then the window will not be initialized with the traits
    // determined in the ".ui" file associated with this window.
    ui->setupUi(this);

    ts = nullptr;
}

EFIDAQ::~EFIDAQ()
{
    delete ui;
}

void EFIDAQ::on_pushButton_clicked()
{
    // Do not need to worry about cleaning this up when the window is closed if
    // the function "this->setAttribute(Qt::WA_DeleteOnClose)" is called in the
    // constructor of the window that is being created. This functions call will
    // make the GUI automatically call the destructor of the window when it is
    // closed.
    ts = new TESTSETUP();

    // Makes the newly created window visible to the user.
    ts->show();
}
