#include "tmodels.h"
#include "utilities.h"
#include <QBrush>
#include <QFont>
#include <QTextStream>
#include <QFile>
#include <stdio.h>

//================================//
// AFR_TABLE_MODEL IMPLEMENTATION //
//================================//

AFR_TABLE_MODEL::AFR_TABLE_MODEL(QObject *parent)
    :QAbstractTableModel(parent)
{
    // Attempts to load the AFR_TABLE.csv contents into the table.
    // Need to make it so that the user can specify the name of the file
    // to load from. This will be the default value for now.
    if (!loadCSV(":/AFR_TABLE.csv"))
    {
        m_gridData = new QString*[DEFAULT_NROWS];
        m_nrows = DEFAULT_NROWS;
        for (unsigned int i = 0; i < DEFAULT_NROWS; i++)
        {
            m_gridData[i] = new QString[DEFAULT_NCOLS];
        }
        m_ncols = DEFAULT_NCOLS;

        // Initialize the data in the table here if no AFR_TABLE.csv is present
        for (unsigned int i = 0; i < m_nrows; i++)
        {
            for (unsigned int j = 0; j < m_ncols; j++)
            {
                // Defaults to just printing the coordinates of each spot in the table.
                m_gridData[i][j] = QString("(%1, %2)").arg(i).arg(j);
            }
        }
    }
}

AFR_TABLE_MODEL::~AFR_TABLE_MODEL()
{
    for (unsigned int i = 0; i < m_nrows; i++)
    {
        delete[] m_gridData[i];
    }
    delete[] m_gridData;
}

// Determines the number of rows in the table.
int AFR_TABLE_MODEL::rowCount(const QModelIndex & /*parent*/) const
{
    return (int) m_nrows;
}

// Determines the number of columns in the table.
int AFR_TABLE_MODEL::columnCount(const QModelIndex & /*parent*/) const
{
    return (int) m_ncols;
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
        if (orientation == Qt::Horizontal) { // Headers for each column (appear above)
            switch (section)
            {
            case 0:
                return QString("0 - 799");
            case 1:
                return QString("800 - 1599");
            case 2:
                return QString("1600 - 2399");
            case 3:
                return QString("2400 - 3199");
            case 4:
                return QString("3200 - 3999");
            case 5:
                return QString("4000 - 4799");
            case 6:
                return QString("4800 - 5599");
            case 7:
                return QString("5600 - 6399");
            case 8:
                return QString("6400 - 7199");
            case 9:
                return QString("7200 - 7999");
            }
        }
        else if (orientation == Qt::Vertical) { // Headers for each row (appear on the left)
            switch (section)
            {
            case 0:
                return QString("100%");
            case 1:
                return QString("90%");
            case 2:
                return QString("80%");
            case 3:
                return QString("70%");
            case 4:
                return QString("60%");
            case 5:
                return QString("50%");
            case 6:
                return QString("40%");
            case 7:
                return QString("30%");
            case 8:
                return QString("20%");
            case 9:
                return QString("10%");
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
        //save value from editor to member m_gridData
        m_gridData[index.row()][index.column()] = value.toString();

        //for presentation purposes only: build and emit a joined string
        QString result;
        for(unsigned int row = 0; row < m_nrows; row++)
        {
            for(unsigned int col = 0; col < m_ncols; col++)
            {
                result += m_gridData[row][col] + " ";
            }
        }

        // signal that signifies the editing has finished.
        emit editCompleted( result );

        // Need to emit this signal to tell the table to update its values.
        emit dataChanged(index, index);
    }
    return true;
}

// Flags for specific cells.
Qt::ItemFlags AFR_TABLE_MODEL::flags(const QModelIndex & /*index*/) const
{
    //int row = index.row();
    //int column = index.column();

    // This will make all cells in the table editable.
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

// Loads a ".csv" file into the table. Currently only supports a fixed size table.
// Also does not suport changing the headers yet.
bool AFR_TABLE_MODEL::loadCSV(QString filename)
{
    // Open the input filename as read only.
    QFile inputFile(filename);
    inputFile.open(QIODevice::ReadOnly);

    // Check to make sure the file was opened properly
    if (inputFile.isOpen())
    {
        const QChar col_delimiter = ',';
        const QChar row_delimiter = '\n';
        // Possibly need to check if input stream was created successfully?
        QTextStream in(&inputFile);

        // Determine the number of rows that need to be allocated.
        QString alltext = in.readAll();
        m_nrows = numfields(row_delimiter, alltext);

        // Allocate the necessary number of rows.
        m_gridData = new QString*[m_nrows];
        m_ncols = 0;

        // Take the number of fields in the first line as the number of columns
        // in the table.
        in.seek(0);
        QString line = in.readLine();
        m_ncols = numfields(col_delimiter, line);

        // Start back from the beginning of the delimited file.
        in.seek(0);
        for (unsigned int row = 0; row < m_nrows; row++)
        {
            // Read a line from the .csv file
            QString line = in.readLine();

            // Count the number of fields in the line.
            unsigned int ncols = numfields(col_delimiter, line);

            // Check to make sure all input lines have the same number of fields.
            if (ncols != m_ncols)
            {
                for (unsigned int j = 0; j < row; j++)
                {
                    delete[] m_gridData[j];
                }
                delete[] m_gridData;
                return false;
            }

            // Allocate an array of QStrings large enough to hold each column.
            m_gridData[row] = new QString[ncols];

            // Copy the value for each field into the table data.
            for (unsigned int col = 0, i = 0; col < ncols; col++, i++)
            {
                m_gridData[row][col] = "";
                while ((int) i < line.length() && line[i] != col_delimiter)
                {
                    m_gridData[row][col] += line[i];
                    i++;
                }
            }
        }
        // Perform resource cleanup
        inputFile.close();

        // Return true on success
        return true;
    }
    else
    {
        // Return false on failure
        return false;
    }
}

//===================================//
// LIST_CHOICES_MODEL IMPLEMENTATION //
//===================================//

LIST_CHOICES_MODEL::LIST_CHOICES_MODEL(QObject *parent)
    :QAbstractListModel(parent)
{
    // Attempts to load the LABEL_LIST.csv contents into the table.
    // Need to make it so that the user can specify the name of the file
    // to load from. This will be the default value for now.
    if (!loadCSV(":/LABEL_LIST.csv"))
    {
        m_listData = new QString[DEFAULT_NROWS];
        m_listSize = DEFAULT_NROWS;
        // Initialize the data in the table here if no AFR_TABLE.csv is present
        for (unsigned int i = 0; i < DEFAULT_NROWS; i++)
        {
            // Defaults to just printing the coordinates of each spot in the table.
            m_listData[i] = QString("%1").arg(i);
        }
    }
}

LIST_CHOICES_MODEL::~LIST_CHOICES_MODEL()
{
    delete[] m_listData;
}

// Determines the number of rows in the list.
int LIST_CHOICES_MODEL::rowCount(const QModelIndex & /*parent*/) const
{
    return (int) m_listSize;
}

// Sets the parameters that determine what and how data is displayed in the cells
// of the table. The top left corner of the table is row = 0 and column = 0 while the
// bottom right corner of the table is row = rowCount() - 1 and column = columnCount() - 1.
QVariant LIST_CHOICES_MODEL::data(const QModelIndex &index, int role) const
{
    int row = index.row();

    switch(role){
    case Qt::DisplayRole: // Sets the text at the point (row,column) in the table.
        return m_listData[row];
        break;
    case Qt::FontRole: // Sets the type of font at the point (row,column) in the table.
        // Change the font
        return QVariant();
        break;
    case Qt::BackgroundRole:
        // Controls the background color for each cell
        return QVariant();
        break;
    case Qt::TextAlignmentRole:
        // Controls the text alignment for each cell
        return QVariant();
        break;
    case Qt::CheckStateRole:
        // Controls the state of any cells that have checkboxes
        return QVariant();
        break;
    }
    // Return value for all "Roles" that do not have a switch statement to handle them.
    return QVariant();
}

// Controls the text displayed for the header for each row.
QVariant LIST_CHOICES_MODEL::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) { // Headers for each column (appear above)
            switch (section)
            {
            case 0:
                return QString("0 - 799");
            case 1:
                return QString("800 - 1599");
            case 2:
                return QString("1600 - 2399");
            case 3:
                return QString("2400 - 3199");
            case 4:
                return QString("3200 - 3999");
            case 5:
                return QString("4000 - 4799");
            case 6:
                return QString("4800 - 5599");
            case 7:
                return QString("5600 - 6399");
            case 8:
                return QString("6400 - 7199");
            case 9:
                return QString("7200 - 7999");
            }
        }
    }
    return QVariant();
}

// Loads a ".csv" file into the table. Currently only supports a fixed size table.
// Also does not suport changing the headers yet.
bool LIST_CHOICES_MODEL::loadCSV(QString filename)
{
    QFile inputFile(filename);
    inputFile.open(QIODevice::ReadOnly);

    // Check to make sure the file was opened properly
    if (inputFile.isOpen())
    {
        const QChar delimiter = ',';

        // Possibly need to check if input stream was created successfully?
        QTextStream in(&inputFile);

        // Read a line from the .csv file
        QString line = in.readLine();

        m_listSize = numfields(delimiter, line);

        m_listData = new QString[m_listSize];

        for (unsigned int row = 0, i = 0; row < m_listSize; row++, i++)
        {
            while ((int) i < line.length() && line[i] != delimiter)
            {
                m_listData[row] += line[i];
                i++;
            }
        }

        // Perform resource cleanup
        inputFile.close();

        // Return true on success
        return true;
    }
    // Return false on failure to open file
    return false;
}

