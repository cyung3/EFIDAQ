/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2016 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 13.09.16                                             **
**          Version: 2.0.0-beta                                           **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of PlotWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  PlotWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see PlotWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "ui_plotwindow.h"
#include "plotwindow.h"
#include "runtest.h"
#include "utilities.h"

#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

PlotWindow::PlotWindow(QWidget* parent, RUNTEST* rparent) :
  QMainWindow(parent),
  ui(new Ui::PlotWindow),
  m_xData(efidaq::DEFAULT_MAX_PLOTTED_POINTS),
  m_yData(efidaq::DEFAULT_MAX_PLOTTED_POINTS),
  measuredFrameRate(30)
{
  ui->setupUi(this);
  //setGeometry(400, 250, 542, 390);
  
  // Needed in order to set a flag in RUNTEST if the plotting window
  // is abruptly closed.
  this->rparent = rparent;

  // Connect the menu actions to their slots
  connect(ui->actionData_Points, SIGNAL(triggered()), SLOT(handleActionDataPointsTriggered()));
  connect(ui->actionClear, SIGNAL(triggered()), SLOT(handleActionClearTriggered()));
  connect(ui->actionConnect_Points, SIGNAL(triggered(bool)), SLOT(handleActionConnectPointsTriggered(bool)));
  connect(ui->actionSave_Plot, SIGNAL(triggered()), SLOT(handleActionSavePlotTriggered()));
  connect(ui->actionFrame_Rate, SIGNAL(triggered()), SLOT(handleActionFrameRateTriggered()));

  setup(ui->customPlot);
  QString name = "Dynamic Plot";
  setWindowTitle(name);
  statusBar()->clearMessage();
  ui->customPlot->replot();

  time = new QTime;
  time->start();
  lastTime = time->elapsed();
  frameRate = 30;
  secPerFrame = 1.0 / frameRate;

  //setupDemo();
  // for making screenshots of the current demo or all demos (for website screenshots):
  //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
  //QTimer::singleShot(4000, this, SLOT(screenShot()));
}
void PlotWindow::addData(QVector<double> X, QVector<double> Y, QString xLabel, QString yLabel)
{
    if (X.isEmpty() || Y.isEmpty())
        return;

    // add data to lines:
    ui->customPlot->graph(0)->addData(X, Y);

    // add axis labels
    ui->customPlot->xAxis->setLabel(xLabel);
    ui->customPlot->yAxis->setLabel(yLabel);

    //std::sort(X.begin(), X.end());

    // rescale both axes to fit the current data
    ui->customPlot->graph(0)->rescaleAxes();
    bool foundRange = false;
    QCPRange xrange = ui->customPlot->graph(0)->getKeyRange(foundRange);
    if (foundRange)
    {
        ui->customPlot->xAxis->setRange(xrange);
    }

    foundRange = false;
    QCPRange yrange = ui->customPlot->graph(0)->getValueRange(foundRange);
    if (foundRange)
        ui->customPlot->yAxis->setRange(yrange);

    // make key axis range scroll with the data (at a constant range size of 8):
    //ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);

    ui->customPlot->replot();

    // Only keep 100 data points at a time.
    if (ui->customPlot->graph(0)->dataCount() > 100)
    {
        ui->customPlot->graph(0)->data()->clear();
    }
}

void PlotWindow::setData(QVector<double> X, QVector<double> Y, QString xLabel, QString yLabel)
{
    if (X.isEmpty() || Y.isEmpty())
        return;

    m_xData.push(X);
    m_yData.push(Y);

    double deltaTime = (time->elapsed() - lastTime) / 1000.0;
    if (deltaTime >= secPerFrame)
    {
        // add data to lines:
        ui->customPlot->graph(0)->setData(m_xData, m_yData);

        lastTime = time->elapsed();

        // add axis labels
        ui->customPlot->xAxis->setLabel(xLabel);
        ui->customPlot->yAxis->setLabel(yLabel);

        //std::sort(X.begin(), X.end());

        // rescale both axes to fit the current data
        ui->customPlot->graph(0)->rescaleAxes();
        bool foundRange = false;
        QCPRange xrange = ui->customPlot->graph(0)->getKeyRange(foundRange);
        if (foundRange)
        {
            ui->customPlot->xAxis->setRange(xrange);
        }

        foundRange = false;
        QCPRange yrange = ui->customPlot->graph(0)->getValueRange(foundRange);
        if (foundRange)
        {
            ui->customPlot->yAxis->setRange(yrange);
        }

        ui->customPlot->replot();

        measuredFrameRate.push(1 / deltaTime);
        double measure = mean(measuredFrameRate);
        ui->statusBar->showMessage(QString("Measured Frame Rate: %1").arg(measure));
        measuredFrameRate.setMaxSize((int) measure);
        // make key axis range scroll with the data (at a constant range size of 8):
        //ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    }
}

void PlotWindow::setup(QCustomPlot* customPlot)
{
    // include this section to fully disable antialiasing for higher performance:

    customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    customPlot->xAxis->setTickLabelFont(font);
    customPlot->yAxis->setTickLabelFont(font);
    customPlot->legend->setFont(font);

    // Creates the graph
    customPlot->addGraph();

    // Sets the points to blue
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

    //QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    //timeTicker->setTimeFormat("%h:%m:%s");
    //customPlot->xAxis->setTicker(timeTicker);
    //customPlot->axisRect()->setupFullAxesBox();
    //customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void PlotWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

void PlotWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    rparent->stopPlotting();
}

PlotWindow::~PlotWindow()
{
  delete ui;
}

void PlotWindow::handleActionDataPointsTriggered()
{
    int value = m_xData.maxSize();
    value = QInputDialog::getInt(
                this,
                QString("Data Point"),
                QString("Input the maximum number of data points that can be on the plot."),
                value,
                1);
    m_xData.setMaxSize(value);
    m_yData.setMaxSize(value);
}

void PlotWindow::handleActionClearTriggered()
{
    m_xData.clear();
    m_yData.clear();
}

void PlotWindow::handleActionConnectPointsTriggered(bool connect)
{
    if (connect)
    {
        // Sets the line style to be connect data points by a line.
        ui->customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsLine);

        // Sets the scatter style to none for lines.
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, 6));
    }
    else
    {
        // Sets the line style to none, making it a scatter plot.
        ui->customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);

        // Sets the scatter style to be discs.
        ui->customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));
    }

}

void PlotWindow::handleActionSavePlotTriggered()
{

}

void PlotWindow::handleActionFrameRateTriggered()
{
    int value = frameRate;
    value = QInputDialog::getInt(
                this,
                QString("Frame Rate"),
                QString("Input the desired frame rate for refreshing the plot."),
                value,
                1);
    frameRate = value;
    secPerFrame = 1.0 / frameRate;
}

































