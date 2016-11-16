#include "efidaq.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EFIDAQ w;
    w.show();

    return a.exec();
}
