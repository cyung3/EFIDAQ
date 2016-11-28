#include "plotting.h"
#include "ui_plotting.h"
#include <QMessageBox>
#include "tmodels.h"
#include <QTimer>

PLOTTING::PLOTTING(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PLOTTING)
{
    // Sets up the window to look like its ".ui" file.
    ui->setupUi(this);

    // Set the window to automatically call the destructor when closed.
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}
