QT       += core gui widgets quick quickwidgets
CONFIG += c++11

TARGET = qcstyler
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += gallery/resources.qrc
