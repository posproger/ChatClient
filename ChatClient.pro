#-------------------------------------------------
#
# Project created by QtCreator 2014-10-02T11:10:05
#
#-------------------------------------------------

QT       += core gui
QT       += websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ccnetmanager.cpp \
    ccchannel.cpp

HEADERS  += mainwindow.h \
    ccnetmanager.h \
    ccchannel.h

FORMS    += mainwindow.ui
