#-------------------------------------------------
#
# Project created by QtCreator 2014-07-02T20:38:53
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_teststest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_teststest.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Orm4Qt/release/ -lOrm4Qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Orm4Qt/debug/ -lOrm4Qt
else:unix: LIBS += -L$$OUT_PWD/../Orm4Qt/ -lOrm4Qt

INCLUDEPATH += $$PWD/../Orm4Qt
DEPENDPATH += $$PWD/../Orm4Qt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Orm4Qt/release/libOrm4Qt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Orm4Qt/debug/libOrm4Qt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Orm4Qt/release/Orm4Qt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Orm4Qt/debug/Orm4Qt.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Orm4Qt/libOrm4Qt.a
