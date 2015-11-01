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
    myOPC/impl/work_mode.cpp \
    myOPC/impl/cache/memory.cpp

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
    myOPC/impl/work_mode.h \
    myOPC/impl/cache/cache.h \
    myOPC/impl/cache/def.h \
    myOPC/impl/cache/memory.h

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
    viewer.ui

OTHER_FILES += devices/*.json\
    тз/*\
