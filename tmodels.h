#ifndef TMODELS_H
#define TMODELS_H

#include <QAbstractTableModel>

const int ROWS = 10;
const int COLS = 10;

class AFR_TABLE_MODEL : public QAbstractTableModel
{
    Q_OBJECT
public:
    AFR_TABLE_MODEL(QObject *parent);

    // Determines the number of rows in the table.
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    // Determines the number of columns in the table.
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // Determines the initial text in each cell in the table.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Determines the initial headers over each column and to the left of each row.
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // Allows the user to enter text into the table.
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    // Flags that change properties about specific cells such as which cells can be edited.
    Qt::ItemFlags flags(const QModelIndex & index) const ;
private:
    // holds text entered into the table cells
    QString m_gridData[ROWS][COLS];
signals:
    void editCompleted(const QString &);
};

#endif // TMODELS_H
