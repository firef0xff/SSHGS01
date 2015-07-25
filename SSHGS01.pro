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
    test_case/test_case.cpp \
    test_case/test.cpp \
    test_case/tests.cpp \
    hydro_title_info.cpp \
    devices/device_collection.cpp \
    devices/device.cpp \
    servo_title_info.cpp \
    test_form.cpp \
    test_runner.cpp

HEADERS  += mainwindow.h \
    test_case/test_case.h \
    test_case/test.h \
    test_case/tests.h \
    hydro_title_info.h \
    devices/device_collection.h \
    devices/device.h \
    servo_title_info.h \
    test_form.h \
    test_runner.h

FORMS    += mainwindow.ui \
    hydro_title_info.ui \
    servo_title_info.ui \
    test_form.ui \
    test_runner.ui
