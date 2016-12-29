#ifndef RUNTEST_H
#define RUNTEST_H

#include <QWidget>
#include <QVector>
#include <QByteArray>

#include "tmodels.h"

namespace Ui {
class RUNTEST;
}

class PlotWindow;
class SERIALREADER;
class AFRTABLE;

class RUNTEST : public QWidget
{
    Q_OBJECT

public:
    RUNTEST(QWidget *parent = 0);
    ~RUNTEST();

    int saveData();
    int clearData();
    void setDataLocked(bool yes = true);
    void stopPlotting(PlotWindow* sender);
public slots:
    void hitDataTimer();
    void xItemChanged(QModelIndex xindex);
    void yItemChanged(QModelIndex yindex);

private slots:
    void on_sampleRateSlider_sliderMoved(int position);
    void on_sampleRateEdit_editingFinished();
    void on_StartDCButton_clicked();
    void on_EndDCButton_clicked();
    void on_OpenAFRTableButton_clicked();
    void on_PlotDataButton_clicked();
    void on_setSerialPortButton_clicked();

private:
    void addData();

    Ui::RUNTEST *ui;
    LIST_CHOICES_MODEL* m_xlmodel;
    LIST_CHOICES_MODEL* m_ylmodel;
    AFRTABLE* m_afrtable;

    QTimer* m_dataRefrTimer;
    unsigned long long int m_ndp;
    QVector<PlotWindow*> pw;
    SERIALREADER* m_serialReader;
    QByteArray* m_bytebuf;
    int expectedNumFields;

    QVector<QVector<double>> m_xData;
    QVector<QVector<double>> m_yData;
    std::pair<QString, int> m_xLabel;
    std::pair<QString, int> m_yLabel;
};

#endif // RUNTEST_H
