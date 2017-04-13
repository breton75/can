#-------------------------------------------------
#
# Project created by QtCreator 2016-02-20T12:22:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = can_test_2
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    sv_can.cpp

HEADERS  += mainwindow.h \
    sv_can.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
