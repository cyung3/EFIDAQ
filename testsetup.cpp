#include "testsetup.h"
#include "ui_testsetup.h"
#include <QMessageBox>
#include <QThread>
#include "tmodels.h"

TESTSETUP::TESTSETUP(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TESTSETUP)
{
    ui->setupUi(this);

    // Make it so that the destructor for this class is automatically closed
    // when the window for this interface is closed. Very useful attribute to
    // set in order to not have to worry about freeing the data when a window
    // is closed.
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // I created a model for the program to construct a table from. It just
    // contains some basic functions that the program uses to construct the
    // columns and rows of the table. This is implemented in the "tmodels.h"
    // header.
    afr_tm = new AFR_TABLE_MODEL(this, ":/AFR_TABLE.csv");
    ui->AFR_tableview->setModel(afr_tm);
    ui->AFR_tableview->show();
}

TESTSETUP::~TESTSETUP()
{
    delete ui;
    delete afr_tm;

    // This will make a message box pop up when the destructor is called
    // for this class. This proves that the setAttribute(Qt::WA_DeleteOnClose)
    // works as intended.
    QString text = "Deleted TESTSETUP";
    QMessageBox qm(nullptr);
    qm.setText(text);
    qm.exec();
}
