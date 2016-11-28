#ifndef RUNTEST_H
#define RUNTEST_H

#include <QWidget>
#include "tmodels.h"

namespace Ui {
class RUNTEST;
}

class PLOTTING;

class RUNTEST : public QWidget
{
    Q_OBJECT

public:
    RUNTEST(QWidget *parent = 0);
    ~RUNTEST();

public slots:
    void hitDataTimer();

private slots:
    void on_sampleRateSlider_sliderMoved(int position);

    void on_sampleRateEdit_editingFinished();

    void on_StartDCButton_clicked();

    void on_EndDCButton_clicked();

    void on_OpenAFRTableButton_clicked();

    void on_PlotDataButton_clicked();

private:
    void addData(int xData, int yData);

    Ui::RUNTEST *ui;
    LIST_CHOICES_MODEL* m_xlmodel;
    LIST_CHOICES_MODEL* m_ylmodel;

    QTimer* m_dataRefrTimer;
    long long m_ndp;
    PLOTTING* plot;
    bool m_isplotting;
    //QCustomPlot* m_chart;
};

#endif // RUNTEST_H
