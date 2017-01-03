#include "efidaq.h"
#include "sender.h"

#include <QApplication>

//#define DEBUG_UDP_MODE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EFIDAQ w;
    w.show();

#ifdef DEBUG_UDP_MODE
    Sender sender;
    sender.show();
#endif

    return a.exec();
}
