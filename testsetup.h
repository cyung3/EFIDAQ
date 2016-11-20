#ifndef TESTSETUP_H
#define TESTSETUP_H

#include <QWidget>

namespace Ui {
class TESTSETUP;
}
class AFR_TABLE_MODEL;

class TESTSETUP : public QWidget
{
    Q_OBJECT

public:
    TESTSETUP(QWidget *parent = 0);
    ~TESTSETUP();

private:
    Ui::TESTSETUP* ui;
    AFR_TABLE_MODEL* afr_tm;
};

#endif // TESTSETUP_H
