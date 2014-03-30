#-------------------------------------------------
#
# Project created by QtCreator 2014-03-13T18:11:14
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = symuluj
TEMPLATE = app


SOURCES += main.cpp\
    qcustomplot/qcustomplot.cpp \
    obiektsiso.cpp \
    obiektdyskretny.cpp \
    gui.cpp \
    qvectorvalidator.cpp \
    qkontrolersymulacji.cpp

HEADERS  += \
    qcustomplot.h \
    obiektsiso.h \
    obiektdyskretny.h \
    qkontrolersymulacji.h \
    gui.h \
    stale.h \
    qvectorvalidator.h

FORMS    += \
    gui.ui
