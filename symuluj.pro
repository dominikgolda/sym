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
    qkontrolersymulacji.cpp \
    dekorator/dekorator.cpp \
    dekorator/komponent.cpp \
    dekorator/dekorujtrojkatem.cpp \
    dekorator/dekorujstala.cpp \
    dekorator/dekorujsinusem.cpp \
    dekorator/dekorujprostokatem.cpp \
    dekorator/komponentkonkretny.cpp \
    regulator/kompozytobiektow.cpp \
    regulator/regulator.cpp \
    regulator/petlaregulacji.cpp \
    regulator/regulatorp.cpp

HEADERS  += \
    qcustomplot.h \
    obiektsiso.h \
    obiektdyskretny.h \
    qkontrolersymulacji.h \
    gui.h \
    stale.h \
    qvectorvalidator.h \
    dekorator/dekorujtrojkatem.h \
    dekorator/dekorujstala.h \
    dekorator/dekorujsinusem.h \
    dekorator/dekorujprostokatem.h \
    dekorator/dekorator.h \
    dekorator/komponent.h \
    dekorator/komponentkonkretny.h \
    regulator/kompozytobiektow.h \
    regulator/regulator.h \
    regulator/petlaregulacji.h \
    regulator/regulatorp.h

FORMS    += \
    gui.ui
