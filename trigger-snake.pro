#-------------------------------------------------
#
# Project created by QtCreator 2018-03-03T11:46:37
#
#-------------------------------------------------

QT       += core gui

TARGET = trigger-snake
TEMPLATE = app


SOURCES += main.cpp\
    Alea.cpp \
    LCDRange.cpp \
    Board.cpp \
    Helpers.cpp

HEADERS  += \
    Alea.h \
    MersenneTwister.h \
    LCDRange.h \
    Board.h \
    Helpers.h

FORMS    += mainwindow.ui
