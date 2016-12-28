#ifndef UTILITIES_H
#define UTILITIES_H

#include <QList>
#include <QString>
#include <QChar>

namespace efidaq
{
const int SUCCESS = 0;
const int CANCELLED = 1;
const int FAILED_CURRENTLY_COLLECTING = 2;
const int OPEN_FILE_FAILED = 3;
const int WRITE_FILE_FAILED = 4;

const int DEFAULT_MAX_PLOTTED_POINTS = 100;
}

unsigned int numfields(const QChar delimiter, const QString input);

bool loadCSV(QString filename, QList<QList<QString>>& allFields);

void notify(QString msg);
void notify(QList<QString> msg);
void notify(QList<QList<QString>> msg);

double mean(QVector<double> in);

#endif // UTILITIES_H
