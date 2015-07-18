#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T23:03:31
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SSHGS01
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    defines/examinee.cpp \
    title_info.cpp \
    test_case/test_case.cpp \
    test_case/test.cpp \
    test_case/tests.cpp

HEADERS  += mainwindow.h \
    defines/examinee.h \
    title_info.h \
    test_case/test_case.h \
    test_case/test.h \
    test_case/tests.h

FORMS    += mainwindow.ui \
    title_info.ui
