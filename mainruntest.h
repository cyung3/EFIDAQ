#ifndef MAINRUNTEST_H
#define MAINRUNTEST_H

#include <QMainWindow>

namespace Ui {
class MAINRUNTEST;
}

class RUNTEST;

class MAINRUNTEST : public QMainWindow
{
    Q_OBJECT

public:
    MAINRUNTEST(QWidget *parent = 0);
    ~MAINRUNTEST();

    void closeEvent(QCloseEvent *event);
private slots:
    bool saveData();
    bool clearData();
    void dataLockChanged(bool checked);
    void handleParametersEditTriggered();

private:
    Ui::MAINRUNTEST *ui;

    RUNTEST* rt;
};

#endif // MAINRUNTEST_H
