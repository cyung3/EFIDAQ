#include "tmodels.h"
#include <QBrush>
#include <QFont>

AFR_TABLE_MODEL::AFR_TABLE_MODEL(QObject *parent)
    :QAbstractTableModel(parent)
{
    // Initialize the data in the table here.
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            m_gridData[i][j] = QString("(%1, %2)").arg(i).arg(j);
        }
    }
}

// Determines the number of rows in the table.
int AFR_TABLE_MODEL::rowCount(const QModelIndex & /*parent*/) const
{
    return ROWS;
}

// Determines the number of columns in the table.
int AFR_TABLE_MODEL::columnCount(const QModelIndex & /*parent*/) const
{
    return COLS;
}

// Sets the parameters that determine what and how data is displayed in the cells
// of the table. The top left corner of the table is row = 0 and column = 0 while the
// bottom right corner of the table is row = rowCount() - 1 and column = columnCount() - 1.
QVariant AFR_TABLE_MODEL::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();

    switch(role){
    case Qt::DisplayRole: // Sets the text at the point (row,column) in the table.
        return m_gridData[row][col];
        break;
    case Qt::FontRole: // Sets the type of font at the point (row,column) in the table.
        if (row == 0 && col == 0) //change font only for cell(0,0)
        {
            QFont boldFont;
            boldFont.setBold(true);
            return boldFont;
        }
        break;
    case Qt::BackgroundRole:

        if (row == 1 && col == 2)  //change background only for cell(1,2)
        {
            QBrush redBackground(Qt::red);
            return redBackground;
        }
        break;
    case Qt::TextAlignmentRole:

        if (row == 1 && col == 1) //change text alignment only for cell(1,1)
        {
            return Qt::AlignRight + Qt::AlignVCenter;
        }
        break;
    case Qt::CheckStateRole:

        if (row == 1 && col == 0) //add a checkbox to cell(1,0)
        {
            return Qt::Checked;
        }
        break;
    }
    return QVariant();
}

// Controls the text displayed for the header over each column and row.
QVariant AFR_TABLE_MODEL::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("first");
            case 1:
                return QString("second");
            case 2:
                return QString("third");
            }
        }
        else if (orientation == Qt::Vertical) {
            switch (section)
            {
            case 0:
                return QString("first");
            case 1:
                return QString("second");
            case 2:
                return QString("third");
            }
        }
    }
    return QVariant();
}

// Allows the user to edit data in the table.
bool AFR_TABLE_MODEL::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        m_initializing = false;

        //save value from editor to member m_gridData
        m_gridData[index.row()][index.column()] = value.toString();

        //for presentation purposes only: build and emit a joined string
        QString result;
        for(int row= 0; row < ROWS; row++)
        {
            for(int col= 0; col < COLS; col++)
            {
                result += m_gridData[row][col] + " ";
            }
        }

        // signal that signifies the editing has finished.
        emit editCompleted( result );
        emit dataChanged(index, index);
    }
    return true;
}

// Flags for specific cells.
Qt::ItemFlags AFR_TABLE_MODEL::flags(const QModelIndex & index) const
{
    int row = index.row();
    int column = index.column();

    // This will make all cells in the table editable.
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}
