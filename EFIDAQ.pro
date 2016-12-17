#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T16:54:29
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = EFIDAQ
TEMPLATE = app


SOURCES += main.cpp\
        efidaq.cpp \
    testsetup.cpp \
    tmodels.cpp \
    runtest.cpp \
    utilities.cpp \
    qcustomplot.cpp \
    mainwindow.cpp \
    serialreader.cpp \
    mainruntest.cpp \
    afrtable.cpp

HEADERS  += efidaq.h \
    testsetup.h \
    tmodels.h \
    runtest.h \
    utilities.h \
    qcustomplot.h \
    mainwindow.h \
    serialreader.h \
    mainruntest.h \
    afrtable.h

FORMS    += efidaq.ui \
    testsetup.ui \
    runtest.ui \
    mainwindow.ui \
    mainruntest.ui \
    afrtable.ui

RESOURCES += \
    efidaq.qrc

DISTFILES += \
    TODO.txt
