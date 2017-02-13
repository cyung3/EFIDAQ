#ifndef AFRTABLE_H
#define AFRTABLE_H

#include <QMainWindow>
#include <serialreader.h>

namespace Ui {
class AFRTABLE;
}

class AFR_TABLE_MODEL;

class AFRTABLE : public QMainWindow
{
    Q_OBJECT

public:
    AFRTABLE(QWidget *parent = 0);
    ~AFRTABLE();

private slots:
    void loadTable();

private:
    Ui::AFRTABLE *ui;

    AFR_TABLE_MODEL* m_tmodel;
};

#endif // AFRTABLE_H
