#include "utilities.h"
#include <QChar>
#include <QString>

// Counts the number of fields in a delimited list. If the last field does
// not have a trailing delimiter, treats it as if there is one.
unsigned int numfields(const QChar delimiter, const QString input)
{
    unsigned int num = 0;
    unsigned int i = 0;

    // Counts the number of delimiters. If there is a trailing delimiter,
    // this will be the number of fields. If there is no trailing delimiter,
    // this will be one less than the number of fields.
    while (i < input.length())
    {
        if (input[i] == delimiter)
        {
            num++;
        }
        i++;
    }

    // If there is a trailing comma, the last field was correctly counted.
    // Simply return the counted number.
    if (input[input.length() - 1] == delimiter)
    {
        return num;
    }

    // No trailing delimiter means the last field wasn't counted. Therefore, add 1
    // to the counted number.
    return num + 1;
}
