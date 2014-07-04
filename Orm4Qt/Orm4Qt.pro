#-------------------------------------------------
#
# Project created by QtCreator 2014-07-02T20:36:50
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = Orm4Qt
TEMPLATE = lib
CONFIG += staticlib

SOURCES += orm4qt.cpp

HEADERS += orm4qt.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
