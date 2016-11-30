#include "runtest.h"
#include "ui_runtest.h"
#include "serialreader.h"
#include <QMessageBox>
#include "tmodels.h"
#include "mainwindow.h"
#include <QTimer>

RUNTEST::RUNTEST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RUNTEST)
{
    // Sets up the window to look like its ".ui" file.
    ui->setupUi(this);

    // Set the window to automatically call the destructor when closed.
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    // Initialize the number of data points collected as 0.
    m_ndp = 0;

    // Initialize as not plotting
    m_isplotting = false;

    // Sets the xlistView to the LIST_CHOICES_MODEL.
    m_xlmodel = new LIST_CHOICES_MODEL(this);
    ui->xlistView->setModel(m_xlmodel);

    // Sets the ylistView to the LIST_CHOICES_MODEL.
    m_ylmodel = new LIST_CHOICES_MODEL(this);
    ui->ylistView->setModel(m_ylmodel);

    // Sets the source document for the DataBrowser to the specified file.
    //ui->DataBrowser->setSource(QUrl("qrc:/INPUT_DATA.csv"));

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
}

// Destructor for the RUNTEST class
RUNTEST::~RUNTEST()
{
    delete ui;
    delete m_xlmodel;
    delete m_ylmodel;
    delete m_dataRefrTimer;

    QString text = "Deleted RUNTEST";
    QMessageBox qm(nullptr);
    qm.setText(text);
    qm.exec();
}

// Function that runs everytime the time triggers.
void RUNTEST::hitDataTimer()
{
    // Thought this was supposed to update the DataBrowser to the newest
    // version of its source document, but apparently something is not
    // working as I expected it to.
    //ui->DataBrowser->reload();

    // The following commented out statement is how text would be
    // written to the DataBrowser without using the source document.
    //ui->DataBrowser->setPlainText("Hello");

    // The following commented out statement is how text would be appended
    // to the end of the databrowser.
    //ui->DataBrowser->append(QString("Data Point: %1").arg(m_ndp));

    // Not sure if there will be issues here if the serialreader happens to be
    // recording bytes while this code is run. Will need to look into further.
    // Possibly will require some sort of protocols to define the beginning and
    // ends of information.
    QByteArray data;
    unsigned long long nBytes = m_serialReader->availableData(data);

    // Check to see if any bytes were recorded
    if (nBytes <= 0)
    {
        // If no bytes are available, then just return because there is no work to be done.
        return;
    }

    // Just to see what it looks like when the byte array is simply appended as a string.
    ui->DataBrowser->append(QString(data));

    // For now, works like this.
    m_ndp++;

    // Update the LCD with the total sampe number
    ui->NumDPlcd->display((int) m_ndp);

    int xData = 0;
    int yData = 0;

    // Update charts
    if (m_isplotting)
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
        QMessageBox errorMSGBOX;
        errorMSGBOX.setText(QString("Failed to open the serial connection"));
        errorMSGBOX.exec();
    }
}

// Activates whenever the End Data Collection Button is clicked.
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
    }
    else
    {
        QMessageBox errorMSGBOX;
        errorMSGBOX.setText(QString("Failed to close the serial connection"));
        errorMSGBOX.exec();
    }
}

// Activates whenever the Open AFR Table button is clicked.
void RUNTEST::on_OpenAFRTableButton_clicked()
{

}

void RUNTEST::addData(int xData, int yData)
{
    mw->addData(xData,yData);
}

// Activates whenever the Plot Data button is clicked.
void RUNTEST::on_PlotDataButton_clicked()
{
    m_isplotting = true;
    mw = new MainWindow();
    mw->show();
}

void RUNTEST::on_setSerialPortButton_clicked()
{
    ui->serialPortNameLabel->setText(m_serialReader->selectPort());
}
