#include "runtest.h"
#include "ui_runtest.h"
#include "serialreader.h"
#include "tmodels.h"
#include "mainwindow.h"
#include "utilities.h"
#include "afrtable.h"
#include <QTimer>
#include <QModelIndex>
#include <QMessageBox>
#include <QPushButton>
#include <QVector>
#include <QFileDialog>
#include <QDir>
#include <QFile>

// Constructor
RUNTEST::RUNTEST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RUNTEST)
{
    // Sets up the window to look like its ".ui" file.
    ui->setupUi(this);

    // Initialize the number of data points collected as 0.
    m_ndp = 0;

    // Initialize as not plotting
    m_isplotting = false;
    // Initialize pointer to plotting window
    mw = nullptr;

    // Locks the data stream from being scrolled through or edited.
    ui->DataBrowser->setDisabled(true);

    // Sets the xlistView to the LIST_CHOICES_MODEL.
    m_xlmodel = new LIST_CHOICES_MODEL(this);
    ui->xlistView->setModel(m_xlmodel);
    connect(ui->xlistView, SIGNAL(clicked(QModelIndex)), SLOT(xItemChanged(QModelIndex)));
    connect(ui->xlistView, SIGNAL(activated(QModelIndex)), SLOT(xItemChanged(QModelIndex)));
    // Initializes the default axes labels by taking the elements at the first index
    // in the model. If no element exists, defaults to an empty label.
    m_xLabelIndex = 0;
    if (!m_xlmodel->getItemAt(0, m_xLabel))
    {
        m_xLabel = "";
    }

    // Sets the ylistView to the LIST_CHOICES_MODEL.
    m_ylmodel = new LIST_CHOICES_MODEL(this);
    ui->ylistView->setModel(m_ylmodel);
    connect(ui->ylistView, SIGNAL(clicked(QModelIndex)), SLOT(yItemChanged(QModelIndex)));
    connect(ui->ylistView, SIGNAL(activated(QModelIndex)), SLOT(yItemChanged(QModelIndex)));
    // Initializes the default axes labels by taking the elements at the first index
    // in the model. If no element exists, defaults to an empty label.
    m_yLabelIndex = 0;
    if (!m_ylmodel->getItemAt(0, m_yLabel))
    {
        m_yLabel = "";
    }
    expectedNumFields = m_ylmodel->rowCount();

    // Sets the max and minimum values on the slider
    // This is actually the maximum sample rate speed using the current method.
    // We will need more advanced programming to get the sample rate higher than
    // 40 samples per second.
    ui->sampleRateSlider->setMaximum(1000);
    ui->sampleRateSlider->setMinimum(1);

    // Sets precision on slider.
    ui->sampleRateSlider->setTickInterval(1);

    // Sets up a time that is supposed to update the textbrowser with any new
    // text in the DataBrowser source document, however, cannot get this to
    // work. May need to implement different method for updating the text
    // browser.
    m_dataRefrTimer = new QTimer(this);

    // This kind of timer is automatically repeating.
    // The interval is in milliseconds. Would want this
    // time to actually be set by the sample rate.
    m_dataRefrTimer->setInterval( (int) ((((double)1/ui->sampleRateSlider->minimum())) * 1000));

    // Connects the timer's "timeout()" signal to the specified slot, which is
    // a function of the RUNTEST class.
    connect(m_dataRefrTimer, SIGNAL(timeout()), this, SLOT(hitDataTimer()));

    // Initialize the End Data Collection Button as disable
    ui->EndDCButton->setDisabled(true);

    // Initialize serial reader for taking in data
    m_serialReader = new SERIALREADER(this);
    ui->serialPortNameLabel->setText(m_serialReader->portName());

    // Initialize the byte buffer for serial input
    m_bytebuf = new QByteArray;
}

// Destructor for the RUNTEST class
RUNTEST::~RUNTEST()
{
    if (m_dataRefrTimer->isActive())
    {
        on_EndDCButton_clicked();
    }
    delete ui;
    delete m_xlmodel;
    delete m_ylmodel;
    delete m_dataRefrTimer;
    delete m_bytebuf;

    if (mw != nullptr)
        delete mw;

    //notify("Deleted RUNTEST");
}

// Stops plotting
void RUNTEST::stopPlotting()
{
    m_isplotting = false;
    delete mw;
    mw = nullptr;
}

// Saves the current data in the DataBrowser.
// Requires the system to not currently be taking data.
int RUNTEST::saveData()
{
    if (m_dataRefrTimer->isActive())
    {
        return efidaq::FAILED_CURRENTLY_COLLECTING;
    }
    else
    {
        QDir dir;
        QString filename = QFileDialog::getSaveFileName(this, QString("Select a file to save the data to."), dir.currentPath());
        if (filename.isEmpty())
        {
            return efidaq::CANCELLED;
        }
        QByteArray data;
        data.append(ui->DataBrowser->toPlainText());
        QFile file(filename, this);
        if (!file.open(QFileDevice::WriteOnly))
        {
            return efidaq::OPEN_FILE_FAILED;
        }
        if (file.write(data) == -1)
        {
            file.close();
            return efidaq::WRITE_FILE_FAILED;
        }
        file.close();
        return efidaq::SUCCESS;
    }
}

// Clears the information in the DataBrowser.
// Requires the system to not currently be taking data.
int RUNTEST::clearData()
{
    if (m_dataRefrTimer->isActive())
    {
        return efidaq::FAILED_CURRENTLY_COLLECTING;
    }
    else
    {
        QMessageBox msgbox;
        msgbox.setText("Are you sure you want to clear the current data stream?");
        QAbstractButton* a = msgbox.addButton(QMessageBox::Yes);
        msgbox.addButton(QMessageBox::Cancel);
        msgbox.exec();
        QAbstractButton* b = msgbox.clickedButton();
        if (b == a)
        {
            ui->DataBrowser->clear();
            m_ndp = 0;
            ui->NumDPlcd->display((int) m_ndp);
            ui->StartDCButton->setText("Start Data Collection");
            return efidaq::SUCCESS;
        }
        else
        {
            return efidaq::CANCELLED;
        }
    }
}

// Toggle the data browser lock.
// True means scrollable and editable.
// False means the opposite.
void RUNTEST::setDataLocked(bool yes)
{
    ui->DataBrowser->setDisabled(yes);
    ui->DataBrowser->setReadOnly(yes);
}

// Function that runs everytime the timer triggers.
void RUNTEST::hitDataTimer()
{
    // Not sure if there will be issues here if the serialreader happens to be
    // recording bytes while this code is run. Will need to look into further.
    // Possibly will require some sort of protocols to define the beginning and
    // ends of information.
    QByteArray data(*m_bytebuf);
    unsigned long long nBytes = m_serialReader->availableData(data);
    m_bytebuf->clear();

    // Check to see if any bytes were recorded
    if (nBytes <= 0 && data.isEmpty())
    {
        // If no bytes are available and there are no bytes waiting to be recorded,
        // then just return because there is no work to be done.
        return;
    }

    // Identify the newline and delimiter character being used
    const char newline = '\n';
    const char delimiter = ',';

    // Split all the data into individual data points by splitting along the newline character.
    QList<QByteArray> indivDataPoints = data.split(newline);
    QList<QByteArray> indivFields;

    // Initialize data lists for the x and y axes respectively.
    QVector<double> xData;
    QVector<double> yData;

    // Loops through the individual data points and prints them to the screen one at a time.
    for (int i = 0; i < indivDataPoints.length()-1; i++)
    {
        indivFields = indivDataPoints[i].split(delimiter);

        // Might not want this but it only prints data points that have the expected
        // number of fields. Filters out some bad input but still not fullproof.
        if (indivFields.length() == expectedNumFields)
        {
            ui->DataBrowser->insertPlainText(QString(indivDataPoints[i]));

            // Makes it only autoscroll if the verticalScrollBar is within 50 lines of the bottom.
            if (ui->DataBrowser->verticalScrollBar()->maximum() - ui->DataBrowser->verticalScrollBar()->value() <= 50)
                ui->DataBrowser->verticalScrollBar()->setValue(ui->DataBrowser->verticalScrollBar()->maximum());

            if (m_isplotting)
            {
                // Appends the xData and yData points
                // Check is necessary to ensure index is within the range of data input.
                if (m_xLabelIndex < indivFields.length() && m_yLabelIndex < indivFields.length())
                {
                    QString xstr = QString(indivFields[m_xLabelIndex]);
                    xstr.remove(QRegExp(QString("[\n\t\r]*")));
                    double xval = xstr.toDouble();
                    xData.append(xval);

                    QString ystr = QString(indivFields[m_yLabelIndex]);
                    ystr.remove(QRegExp(QString("[\n\t\r]*")));
                    double yval = ystr.toDouble();
                    yData.append(yval);
                }
            }
            m_ndp++;
        }
    }


    // If the last entry in the data point list is empty, then the data gathered was a complete point.
    // Otherwise, store the excess data in a buffer to be prepended before the next read from the
    // available serial data.
    if (!indivDataPoints.isEmpty())
    {
        bool isCompleteMsg = indivDataPoints[indivDataPoints.length() - 1].length() == 0;
        if (!isCompleteMsg)
        {
            m_bytebuf->append(indivDataPoints[indivDataPoints.length() - 1]);
        }
    }
    else return;

    // Update the LCD with the total sample number
    ui->NumDPlcd->display((int) m_ndp);
\

    // Update charts
    if (m_isplotting && !xData.isEmpty() && !yData.isEmpty())
    {
        addData(xData, yData);
    }
}

// Activates whenever the sampleRateSlider is moved.
void RUNTEST::on_sampleRateSlider_sliderMoved(int position)
{
    // Displays the slider position in the DataBrowser.
    //ui->DataBrowser->append(QString("Slider position: %1").arg(position));

    // Update the sampleRateEdit to the slider value
    ui->sampleRateEdit->setText(QString("%1").arg(position));

    // Update the timer interval to match the slider value.
    m_dataRefrTimer->setInterval( (int) ((((double)1/position)) * 1000));
}

// Activates whenever the sampleRateEdit has text entered.
void RUNTEST::on_sampleRateEdit_editingFinished()
{
    // Set the value of the slider to the value input into the sampleRateEdit
    // if the input in the edit is valid.
    QString text = ui->sampleRateEdit->text();
    int value = text.toInt();
    if (value >= ui->sampleRateSlider->minimum() && value <= ui->sampleRateSlider->maximum())
    {
        ui->sampleRateSlider->setValue(value);

        // Update the timer interval to match the new value.
        m_dataRefrTimer->setInterval( (int) ((((double)1/value)) * 1000));
    }
}

// Activates whenever the Start Data Collection Button is clicked.
// Resuming data collection causes strange bytes.
void RUNTEST::on_StartDCButton_clicked()
{
    // Attempt to open the serial connection
    if (m_serialReader->open(QIODevice::ReadOnly))
    {
        // Starts the timer.
        if (!m_dataRefrTimer->isActive())
        {
            m_dataRefrTimer->start();
        }

        // Need to make the button become unclickable at this point
        ui->StartDCButton->setDisabled(true);

        // Needs to make sure the end data collection button is activated
        ui->EndDCButton->setDisabled(false);
    }
    else
    {
        notify(QString("Failed to open the serial connection"));
    }
}

// Activates whenever the End Data Collection Button is clicked.
// Ending data collection causes strange bytes.
void RUNTEST::on_EndDCButton_clicked()
{
    if (m_serialReader->close())
    {
        // Stops the timer
        if (m_dataRefrTimer->isActive())
        {
            m_dataRefrTimer->stop();
        }

        // Need to make this button become unactive at this point
        ui->EndDCButton->setDisabled(true);

        // Need to make the start data collection button become active. Also changes the text to "resume data collection."
        ui->StartDCButton->setDisabled(false);
        ui->StartDCButton->setText(QString("Resume Data Collection"));

        // Write any bytes waiting in the byte buffer to the screen
        //ui->DataBrowser->append(QString(*m_bytebuf));

        // Clear out the byte buffer
        m_bytebuf->clear();
    }
    else
    {
        notify("Failed to close the serial connection");
    }
}

// Activates whenever the Open AFR Table button is clicked.
void RUNTEST::on_OpenAFRTableButton_clicked()
{
    // This window is set to delete automatically.
    m_afrtable = new AFRTABLE;
    m_afrtable->show();
}

// Add Data to the plot
void RUNTEST::addData(QVector<double> xData, QVector<double> yData)
{
    if (mw != nullptr)
        mw->addData(xData,yData, m_xLabel, m_yLabel);
}

// Activates whenever the Plot Data button is clicked.
void RUNTEST::on_PlotDataButton_clicked()
{
    m_isplotting = true;
    if (mw == nullptr)
        mw = new MainWindow(nullptr, this);
    mw->show();
}

// Activates whenever the get serial port button has been clicked.
void RUNTEST::on_setSerialPortButton_clicked()
{
    ui->serialPortNameLabel->setText(m_serialReader->selectPort());
}

// Activated whenever the xListView selection is changed.
void RUNTEST::xItemChanged(QModelIndex xindex)
{
    // Change string for x item
    m_xlmodel->getItemAt(xindex.row(), m_xLabel);
    m_xLabelIndex = xindex.row();
    //ui->DataBrowser->append(m_xLabel);
}

// Activated whenever the yListView selection is changed.
void RUNTEST::yItemChanged(QModelIndex yindex)
{
    // change string for y item
    m_ylmodel->getItemAt(yindex.row(), m_yLabel);
    m_yLabelIndex = yindex.row();
    //ui->DataBrowser->append(m_yLabel);
}
