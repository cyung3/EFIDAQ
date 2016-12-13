#ifndef UTILITIES_H
#define UTILITIES_H

#include <QList>
#include <QString>
#include <QChar>

unsigned int numfields(const QChar delimiter, const QString input);

bool loadCSV(QString filename, QList<QList<QString>>& allFields);

void notify(QString msg);
void notify(QList<QString> msg);
void notify(QList<QList<QString>> msg);

#endif // UTILITIES_H
