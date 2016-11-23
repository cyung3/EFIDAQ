#ifndef RUNTEST_H
#define RUNTEST_H

#include <QWidget>
#include "tmodels.h"

namespace Ui {
class RUNTEST;
}

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

private:
    Ui::RUNTEST *ui;
    LIST_CHOICES_MODEL* m_xlmodel;
    LIST_CHOICES_MODEL* m_ylmodel;
    QTimer* m_dataRefrTimer;
    long long m_ndp;
};

#endif // RUNTEST_H
