#include "testsetup.h"
#include "ui_testsetup.h"
#include "utilities.h"
#include "tmodels.h"

#include <QMessageBox>
#include <QThread>

TESTSETUP::TESTSETUP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TESTSETUP)
{
    ui->setupUi(this);
    setWindowTitle("Test Setup");
    setWindowIcon(QIcon(efidaq::DEFAULT_LOGO_FILEPATH));

    // Make it so that the destructor for this class is automatically closed
    // when the window for this interface is closed. Very useful attribute to
    // set in order to not have to worry about freeing the data when a window
    // is closed.
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // I created a model for the program to construct a table from. It just
    // contains some basic functions that the program uses to construct the
    // columns and rows of the table. This is implemented in the "tmodels.h"
    // header.
    afr_tm = new AFR_TABLE_MODEL(this, efidaq::DEFAULT_AFR_TABLE_FILEPATH);
    ui->AFR_tableview->setModel(afr_tm);
    ui->AFR_tableview->show();
}

TESTSETUP::~TESTSETUP()
{
    delete ui;
    delete afr_tm;

    //notify("Deleted TESTSETUP")
}
