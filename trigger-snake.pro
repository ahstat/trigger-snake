#-------------------------------------------------
#
# Project created by QtCreator 2018-03-03T11:46:37
#
#-------------------------------------------------

QT       += core gui

TARGET = trigger-snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lcdrange.cpp \
    Alea.cpp \
    Plateau.cpp

HEADERS  += mainwindow.h \
    lcdrange.h \
    Alea.h \
    MersenneTwister.h \
    Plateau.h

FORMS    += mainwindow.ui
