#include "efidaq.h"
#include "ui_efidaq.h"

EFIDAQ::EFIDAQ(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EFIDAQ)
{
    ui->setupUi(this);
}

EFIDAQ::~EFIDAQ()
{
    delete ui;
}
