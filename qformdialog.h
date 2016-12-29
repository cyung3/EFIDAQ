#ifndef QFORMDIALOG_H
#define QFORMDIALOG_H

#include <QDialog>
#include <QString>
#include <QSet>

namespace Ui {
class QFormDialog;
}

class QLabel;
class QLineEdit;

class QFormDialog : public QDialog
{
    Q_OBJECT

public:
    QFormDialog(QWidget *parent = 0);
    ~QFormDialog();

    void addPair(QString label, QString startVal = QString());
    void addPair(std::pair<QString, QString> pair);
    QSet<std::pair<QString, QString>> getPairs() const;

private slots:
    void handleOkButtonClicked(bool clicked);
    void handleCancelButtonClicked(bool clicked);

private:
    Ui::QFormDialog* ui;
    QSet<std::pair<QString, QString>> m_pairs;
    QList<std::pair<QLabel*, QLineEdit*>> m_deletionList;
};

#endif // QFORMDIALOG_H
