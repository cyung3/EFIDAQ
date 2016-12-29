#include "mainruntest.h"
#include "ui_mainruntest.h"
#include "runtest.h"
#include "utilities.h"
#include "qformdialog.h"

#include <QMessageBox>
#include <QAbstractButton>
#include <QCloseEvent>
#include <QPushButton>
#include <QSet>

MAINRUNTEST::MAINRUNTEST(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MAINRUNTEST)
{
    ui->setupUi(this);
    setWindowTitle(QString("EFI Data Acquisition System"));

    // Delete when closed
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // Create the RUNTEST widget.
    rt = new RUNTEST();

    // Set the RUNTEST widget as the central widget for the MAINRUNTEST object.
    setCentralWidget(rt);

    // Connect Save As menu action to the SaveData function.
    connect(ui->actionSave_As, SIGNAL(triggered()), SLOT(saveData()));

    // Connect Clear menu action to the clearData function.
    connect(ui->actionClear, SIGNAL(triggered()), SLOT(clearData()));

    // Connect Locked menu checkbox action to the dataLockChanged function
    connect(ui->actionLocked, SIGNAL(toggled(bool)), SLOT(dataLockChanged(bool)));

    // Connect the parameter edit menu action to its handler function.
    connect(ui->actionEdit, SIGNAL(triggered()), SLOT(handleParametersEditTriggered()));
}

MAINRUNTEST::~MAINRUNTEST()
{
    delete ui;
    delete rt;
}

bool MAINRUNTEST::saveData()
{
    switch (rt->saveData())
    {
    case efidaq::SUCCESS:
        notify("Data successfully saved.");
        return true;
    case efidaq::CANCELLED:
        notify("Saving cancelled.");
        return false;
    case efidaq::FAILED_CURRENTLY_COLLECTING:
        notify("Data could not be saved. Make sure the system is not currently collecting data.");
        return false;
    case efidaq::OPEN_FILE_FAILED:
        notify("Data could not be saved. Failed to open the specified file for writing.");
        return false;
    case efidaq::WRITE_FILE_FAILED:
        notify("Data could not be saved. Failed to write to specified file.");
        return false;
    default:
        notify("ERROR: Unspecified return value.");
        return false;
    }
}

bool MAINRUNTEST::clearData()
{
    switch (rt->clearData())
    {
    case efidaq::SUCCESS:
        notify("Data successfully cleared.");
        return true;
    case efidaq::CANCELLED:
        notify("Clear command cancelled.");
        return false;
    case efidaq::FAILED_CURRENTLY_COLLECTING:
        notify("Data could not be cleared. Make sure the system is not currently collecting data.");
        return false;
    default:
        notify("ERROR: Unspecified return value.");
        return false;
    }
}

void MAINRUNTEST::dataLockChanged(bool checked)
{
    rt->setDataLocked(checked);
}

void MAINRUNTEST::closeEvent(QCloseEvent *event)
{
    QMessageBox msgbox;
    msgbox.setText("Would you like to save the current data before exiting?");
    QAbstractButton* a = msgbox.addButton(QMessageBox::Yes);
    QAbstractButton* b = msgbox.addButton(QMessageBox::No);
    QAbstractButton* c = msgbox.addButton(QMessageBox::Cancel);
    msgbox.exec();
    QAbstractButton* d = msgbox.clickedButton();
    if (d == a)
    {
        if (saveData())
        {
            event->accept();
        }
        else
        {
            QMessageBox msgbox2;
            msgbox2.setText("Would you still like to exit?");
            QAbstractButton* a = msgbox2.addButton(QMessageBox::Yes);
            QAbstractButton* b = msgbox2.addButton(QMessageBox::No);
            msgbox2.exec();
            QAbstractButton* d = msgbox2.clickedButton();
            if (d == a)
            {
                event->accept();
            }
            else
            {
                event->ignore();
            }
        }
    }
    else if (d == b)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MAINRUNTEST::handleParametersEditTriggered()
{
    QFormDialog pform(this);
    pform.addPair(QString("a"), QString("b"));
    pform.addPair(QString("c"), QString("d"));
    pform.addPair(QString("e"), QString("f"));
    pform.addPair(QString("g"), QString("h"));
    pform.exec();
    QSet<std::pair<QString, QString>> results = pform.getPairs();
    QFormDialog pform2(this);
    for (auto it = results.begin(); it != results.end(); it++)
    {
        pform2.addPair(*it);
    }
    pform2.exec();
}
