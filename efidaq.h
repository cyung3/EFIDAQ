#ifndef EFIDAQ_H
#define EFIDAQ_H

#include <QMainWindow>

namespace Ui {
class EFIDAQ;
}

class TESTSETUP;
class MAINRUNTEST;

class EFIDAQ : public QMainWindow
{
    Q_OBJECT

public:
    explicit EFIDAQ(QWidget *parent = 0);
    ~EFIDAQ();

private slots:
    void on_SetupButton_clicked();

    void on_RunTestButton_clicked();

private:
    Ui::EFIDAQ* ui;
    TESTSETUP* ts;
    MAINRUNTEST* mrt;
};

#endif // EFIDAQ_H
