#-------------------------------------------------
#
# Project created by QtCreator 2015-08-12T15:55:11
#
#-------------------------------------------------

QT       += core gui

QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverlist.cpp \
    addserver.cpp \
    connection.cpp \
    getlist.cpp \
    download.cpp \
    upload.cpp \
    strip.cpp \
    remove.cpp

HEADERS  += mainwindow.h \
    serverlist.h \
    addserver.h \
    connection.h \
    getlist.h \
    download.h \
    upload.h \
    strip.h \
    remove.h

FORMS    += mainwindow.ui \
    serverlist.ui \
    addserver.ui

RESOURCES += \
    img.qrc
