#ifndef TMODELS_H
#define TMODELS_H

#include <QAbstractTableModel>
#include <QAbstractListModel>

const unsigned int DEFAULT_NROWS = 10;
const unsigned int DEFAULT_NCOLS = 10;

class AFR_TABLE_MODEL : public QAbstractTableModel
{
    Q_OBJECT
public:
    AFR_TABLE_MODEL(QObject *parent);
    ~AFR_TABLE_MODEL();

    // Determines the number of rows in the table.
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    // Determines the number of columns in the table.
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    // Determines the text in each cell in the table when it updates.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Determines the initial headers over each column and to the left of each row.
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // Allows the user to enter text into the table.
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    // Flags that change properties about specific cells such as which cells can be edited.
    Qt::ItemFlags flags(const QModelIndex & index) const ;

    // Loads data from a .csv file into the m_gridData member
    bool loadCSV(QString filename);
private:
    // holds text entered into the table cells
    QString** m_gridData;
    unsigned int m_nrows;
    unsigned int m_ncols;
signals:
    void editCompleted(const QString &);
};

class LIST_CHOICES_MODEL : public QAbstractListModel
{
    Q_OBJECT
public:
    LIST_CHOICES_MODEL(QObject *parent);
    ~LIST_CHOICES_MODEL();

    // Determines the number of rows in the list.
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    // Determines the text in each cell of the list every time it updates the list.
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    // Determines the initial headers over each column and to the left of each row.
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // Allows the user to enter text into the table.
    //bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    // Flags that change properties about specific cells such as which cells can be edited.
    //Qt::ItemFlags flags(const QModelIndex & index) const ;

    // Loads data from a .csv file into the m_listData member
    bool loadCSV(QString filename);
private:
    // holds text entered into the table cells
    QString* m_listData;
    unsigned int m_listSize;
signals:
};

#endif // TMODELS_H
