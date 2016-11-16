#ifndef EFIDAQ_H
#define EFIDAQ_H

#include <QMainWindow>

namespace Ui {
class EFIDAQ;
}

class EFIDAQ : public QMainWindow
{
    Q_OBJECT

public:
    explicit EFIDAQ(QWidget *parent = 0);
    ~EFIDAQ();

private:
    Ui::EFIDAQ *ui;
};

#endif // EFIDAQ_H
