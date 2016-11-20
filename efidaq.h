#ifndef EFIDAQ_H
#define EFIDAQ_H

#include <QMainWindow>

namespace Ui {
class EFIDAQ;
}
class TESTSETUP;

class EFIDAQ : public QMainWindow
{
    Q_OBJECT

public:
    explicit EFIDAQ(QWidget *parent = 0);
    ~EFIDAQ();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EFIDAQ* ui;
    TESTSETUP* ts;
};

#endif // EFIDAQ_H
