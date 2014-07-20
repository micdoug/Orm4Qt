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
CONFIG += c++11

SOURCES += \
    reflect.cpp \
    property.cpp \
    log.cpp \
    ormerror.cpp \
    sqlprovider.cpp \
    where.cpp \
    sqliteprovider.cpp \
    standardsqlprovider.cpp \
    postgresqlprovider.cpp

HEADERS += \
    reflect.h \
    property.h \
    propertyconcrete.h \
    class.h \
    annotations.h \
    log.h \
    ormerror.h \
    sqlprovider.h \
    where.h \
    repository.h \
    sqliteprovider.h \
    standardsqlprovider.h \
    postgresqlprovider.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

#DEFINES += ORM4QT_DEBUG_OL
DEFINES += ORM4QT_DEBUG_SL
