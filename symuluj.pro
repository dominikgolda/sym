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
    obiektsiso.cpp \
    obiektdyskretny.cpp \
    qvectorvalidator.cpp \
    dekorator/dekorator.cpp \
    dekorator/komponent.cpp \
    dekorator/dekorujtrojkatem.cpp \
    dekorator/dekorujstala.cpp \
    dekorator/dekorujsinusem.cpp \
    dekorator/dekorujprostokatem.cpp \
    dekorator/komponentkonkretny.cpp \
    regulator/regulator.cpp \
    regulator/petlaregulacji.cpp \
    regulator/regulatorp.cpp \
    gui/qkontrolersymulacji.cpp \
    gui/gui.cpp \
    gui/generatorwartoscizadanej.cpp \
    qcustomplot/qcustomplot.cpp \
    regulator/regulatorpid.cpp \
    regulator/polaczenieszeregowe.cpp \
    klasaxml/klasaxml.cpp \
    gui/nastawypid.cpp

HEADERS  += \
    qcustomplot.h \
    obiektsiso.h \
    obiektdyskretny.h \
    stale.h \
    qvectorvalidator.h \
    dekorator/dekorujtrojkatem.h \
    dekorator/dekorujstala.h \
    dekorator/dekorujsinusem.h \
    dekorator/dekorujprostokatem.h \
    dekorator/dekorator.h \
    dekorator/komponent.h \
    dekorator/komponentkonkretny.h \
    regulator/regulator.h \
    regulator/petlaregulacji.h \
    regulator/regulatorp.h \
    gui/qkontrolersymulacji.h \
    gui/gui.h \
    gui/generatorwartoscizadanej.h \
    regulator/regulatorpid.h \
    regulator/polaczenieszeregowe.h \
    klasaxml/klasaxml.h \
    gui/nastawypid.h

FORMS    += \
    gui.ui
