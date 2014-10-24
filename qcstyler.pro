QT       += core gui widgets quick quickwidgets
CONFIG += c++11

TARGET = qcstyler
TEMPLATE = app


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    newstyledialog.cpp \
    qmlsyntaxhighlighter.cpp \
    style.cpp \
    stylerqmlobject.cpp

HEADERS  += \
    mainwindow.h \
    newstyledialog.h \
    qmlsyntaxhighlighter.h \
    style.h \
    stylerqmlobject.h

FORMS    += \
    mainwindow.ui \
    newstyledialog.ui

RESOURCES += gallery/resources.qrc \
    qml.qrc
