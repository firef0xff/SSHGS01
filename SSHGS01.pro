#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T23:03:31
#
#-------------------------------------------------

QT       += core gui printsupport
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''

TARGET = SSHGS01
TEMPLATE = app

INCLUDEPATH += ../mylib/Widgets\

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
    test_runner.cpp \
    test_case/implementation/hydro/functional_test.cpp \
    stand_params.cpp \
    test_case/test_params.cpp \
    test_case/implementation/test_base.cpp \
    ../mylib/Widgets/RoundDial/round_dial.cpp \
    settings/settings.cpp \
    settings/settings_wnd.cpp \
    devices/device_collection_wnd.cpp \
    control_panels_title_info.cpp \
    hydro_cilinder_title_info.cpp \
    test_case/implementation/test_params_hydro.cpp \
    test_case/implementation/test_params_servo.cpp \
    test_case/implementation/test_params_control_panel.cpp \
    test_case/implementation/test_params_hydro_cilinder.cpp \
    test_case/implementation/hydro/herm_test.cpp \
    test_case/implementation/hydro/pressure_duration_from_expediture.cpp \
    test_case/implementation/hydro/max_expenditure.cpp \
    test_case/implementation/hydro/switchtest.cpp \
    test_case/implementation/hydro/pressure_range.cpp \
    test_case/implementation/hydro/activation_time.cpp \
    test_case/implementation/servo/herm_tests.cpp \
    test_case/implementation/servo/expenditure_from_input.cpp \
    test_case/implementation/servo/expenditure_from_pressure_duration.cpp \
    test_case/implementation/servo/frequency_characteristics.cpp \
    test_case/implementation/servo/transient_performance.cpp \
    test_case/implementation/control_board/power_test.cpp \
    test_case/implementation/control_board/va_characteristic.cpp \
    test_case/implementation/hydro_cylinder/cylinder_test.cpp \
    ../mylib/Widgets/GraphBuilder/graph_builder.cpp \
    viewer.cpp \
    cpu/cpu_memory.cpp \
    cpu/db30.cpp \
    cpu/db32.cpp \
    cpu/db31.cpp \
    myOPC/opc_i.c \
    myOPC/impl/demo_mode.cpp \
    myOPC/impl/cache/memory.cpp \
    myOPC/impl/win_ole_mode.cpp \
    myOPC/impl/sync_thread.cpp \
    login.cpp \
    cpu/db2.cpp \
    cpu/db3.cpp \
    cpu/db9.cpp \
    cpu/db10.cpp \
    cpu/db11.cpp \
    cpu/db22.cpp \
    cpu/db23.cpp \
    cpu/db33.cpp \
    cpu/db34.cpp \
    cpu/db35.cpp \
    cpu/db36.cpp \
    cpu/db37.cpp \
    cpu/db38.cpp \
    cpu/db39.cpp \
    cpu/db40.cpp \
    cpu/db50.cpp \
    cpu/db60.cpp \
    manual_control.cpp \
    cpu/m1.cpp \
    cpu/i1.cpp

HEADERS  += mainwindow.h \
    test_case/test_case.h \
    test_case/test.h \
    test_case/tests.h \
    hydro_title_info.h \
    devices/device_collection.h \
    devices/device.h \
    servo_title_info.h \
    test_form.h \
    test_runner.h \
    test_case/implementation/hydro/functional_test.h \
    stand_params.h \
    test_case/test_params.h \
    test_case/implementation/test_base.h \
    ../mylib/Widgets/RoundDial/round_dial.h \
    settings/settings.h \
    settings/settings_wnd.h \
    devices/device_collection_wnd.h \
    control_panels_title_info.h \
    hydro_cilinder_title_info.h \
    test_case/implementation/test_params_hydro.h \
    test_case/implementation/test_params_servo.h \
    test_case/implementation/test_params_control_panel.h \
    test_case/implementation/test_params_hydro_cilinder.h \
    test_case/implementation/hydro/herm_test.h \
    test_case/implementation/hydro/pressure_duration_from_expediture.h \
    test_case/implementation/hydro/max_expenditure.h \
    test_case/implementation/hydro/switchtest.h \
    test_case/implementation/hydro/pressure_range.h \
    test_case/implementation/hydro/activation_time.h \
    test_case/implementation/servo/herm_tests.h \
    test_case/implementation/servo/expenditure_from_input.h \
    test_case/implementation/servo/expenditure_from_pressure_duration.h \
    test_case/implementation/servo/frequency_characteristics.h \
    test_case/implementation/servo/transient_performance.h \
    test_case/implementation/control_board/power_test.h \
    test_case/implementation/control_board/va_characteristic.h \
    test_case/implementation/hydro_cylinder/cylinder_test.h \
    ../mylib/Widgets/GraphBuilder/graph_builder.h \
    viewer.h \
    cpu/cpu_memory.h \
    cpu/data_block_base.h \
    cpu/db30.h \
    cpu/db32.h \
    cpu/db31.h \
    myOPC/miniOPC.h \
    myOPC/opc.h \
    myOPC/impl/demo_mode.h \
    myOPC/impl/types.h \
    myOPC/impl/cache/cache.h \
    myOPC/impl/cache/def.h \
    myOPC/impl/cache/memory.h \
    myOPC/impl/win_ole_mode.h \
    myOPC/impl/sync_thread.h \
    login.h \
    cpu/db2.h \
    cpu/db3.h \
    cpu/db9.h \
    cpu/db10.h \
    cpu/db11.h \
    cpu/db22.h \
    cpu/db23.h \
    cpu/db33.h \
    cpu/db34.h \
    cpu/db35.h \
    cpu/db36.h \
    cpu/db37.h \
    cpu/db38.h \
    cpu/db39.h \
    cpu/db40.h \
    cpu/db50.h \
    cpu/db60.h \
    manual_control.h \
    cpu/m1.h \
    cpu/i1.h

FORMS    += mainwindow.ui \
    hydro_title_info.ui \
    servo_title_info.ui \
    test_form.ui \
    test_runner.ui \
    stand_params.ui \
    settings/settings_wnd.ui \
    devices/device_collection_wnd.ui \
    control_panels_title_info.ui \
    hydro_cilinder_title_info.ui \
    viewer.ui \
    login.ui \
    manual_control.ui

OTHER_FILES += devices/*.json\
    тз/*\
    config/*\

DISTFILES += \
    тз/ячейки

win32 {
    RC_ICONS += img/Main.ico
    DEFINES += WINDOWS
    LIBS += -lole32 -loleaut32
}

unix {
    DEFINES -= WINDOWS
    DEFINES += DEMO
}

debug {
    DEFINES += DEBUG
}

release {
    DEFINES -= DEBUG
}
#DEFINES += WINDOWS
#DEFINES += DEMO
#DEFINES += DEBUG

RESOURCES +=
