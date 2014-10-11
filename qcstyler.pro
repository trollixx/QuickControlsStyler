QT       += core gui widgets quick quickwidgets
CONFIG += c++11

TARGET = qcstyler
TEMPLATE = app


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qmlsyntaxhighlighter.cpp \
    style.cpp \
    stylemanager.cpp \
    stylerqmlobject.cpp

HEADERS  += \
    mainwindow.h \
    qmlsyntaxhighlighter.h \
    style.h \
    stylemanager.h \
    stylerqmlobject.h

FORMS    += mainwindow.ui

RESOURCES += gallery/resources.qrc
