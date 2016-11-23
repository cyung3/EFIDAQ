#include "runtest.h"
#include "ui_runtest.h"
#include <QMessageBox>
#include "tmodels.h"
#include <QTimer>

RUNTEST::RUNTEST(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RUNTEST)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose, true);

    m_ndp = 0;
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
    ui->DataBrowser->append(QString("Data Point: %1").arg(m_ndp));

    // For now, works like this.
    m_ndp++;

    // Update the LCD with the total sampe number

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
    ui->DataBrowser->append(QString("Slider position: %1").arg(position));

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

void RUNTEST::on_StartDCButton_clicked()
{
    // Starts the timer.
    if (!m_dataRefrTimer->isActive())
    {
        m_dataRefrTimer->start();
    }

    // Need to make the button become unclickable at this point

    // Needs to make sure the end data collection button is activated
}

void RUNTEST::on_EndDCButton_clicked()
{
    // Stops the timer
    if (m_dataRefrTimer->isActive())
    {
        m_dataRefrTimer->stop();
    }

    // Need to make this button become unactive at this point

    // Need to make the start data collection button become active. Maybe change the text to "continue data collection" too?
}

void RUNTEST::on_OpenAFRTableButton_clicked()
{

}

void RUNTEST::addData(int xData, int yData)
{

}

void RUNTEST::on_PlotDataButton_clicked()
{
    m_isplotting = true;
    //m_chart = new QCustomPlot;
}
