#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T13:47:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CMSC124Destiny
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    interpreter.cpp

HEADERS  += mainwindow.h \
    interpreter.h

FORMS    += mainwindow.ui

RESOURCES += \
    Icons.qrc
