#-------------------------------------------------
#
# Project created by QtCreator 2016-04-28T02:04:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = puppies
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gameboard.cpp \
    popupwindow.cpp \
    gamewonwindow.cpp

HEADERS  += mainwindow.h \
    gameboard.h \
    documentation.h \
    popupwindow.h \
    gamewonwindow.h

FORMS    += mainwindow.ui \
    gameboard.ui \
    popupwindow.ui \
    gamewonwindow.ui

RESOURCES += \
    images.qrc

DISTFILES +=
