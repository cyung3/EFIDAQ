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
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#include "mainwindow.h"
#include "runtest.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>

MainWindow::MainWindow(QWidget* parent, RUNTEST* rparent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  
  // Needed in order to set a flag in RUNTEST if the plotting window
  // is abruptly closed.
  this->rparent = rparent;

  setup(ui->customPlot);
  QString name = "Plot";
  setWindowTitle("QCustomPlot: " + name);
  statusBar()->clearMessage();
  ui->customPlot->replot();

  //setupDemo();
  // for making screenshots of the current demo or all demos (for website screenshots):
  //QTimer::singleShot(1500, this, SLOT(allScreenShots()));
  //QTimer::singleShot(4000, this, SLOT(screenShot()));
}

void MainWindow::setupDemo()
{
  setupRealtimeDataDemo(ui->customPlot);
  setWindowTitle("QCustomPlot: "+demoName);
  statusBar()->clearMessage();
  ui->customPlot->replot();
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = "Real Time Data Demo";
  
  // include this section to fully disable antialiasing for higher performance:
/*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
*/
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-1.2, 1.2);
  
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::addData(QVector<double> X, QVector<double> Y, QString xLabel, QString yLabel)
{
    if (X.isEmpty() || Y.isEmpty())
        return;

    // add data to lines:
    ui->customPlot->graph(0)->addData(X, Y);

    // add axis labels
    ui->customPlot->xAxis->setLabel(xLabel);
    ui->customPlot->yAxis->setLabel(yLabel);

    std::sort(X.begin(), X.end());

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

void MainWindow::setup(QCustomPlot* customPlot)
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

    // Sets the line style to none, making it a scatter plot
    //customPlot->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);

    // Sets the scatter style to be dots
    //customPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 6));

    //QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    //timeTicker->setTimeFormat("%h:%m:%s");
    //customPlot->xAxis->setTicker(timeTicker);
    //customPlot->axisRect()->setupFullAxesBox();
    //customPlot->yAxis->setRange(-1.2, 1.2);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    rparent->stopPlotting();
}

MainWindow::~MainWindow()
{
  delete ui;
}







































