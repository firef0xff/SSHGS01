TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    opc_i.c \
    impl/demo_mode.cpp \
    impl/cache/memory.cpp \
    impl/win_ole_mode.cpp

HEADERS += \
    miniOPC.h \
    opc.h \
    impl/demo_mode.h \
    impl/types.h \
    impl/cache/cache.h \
    impl/cache/def.h \
    impl/cache/memory.h \
    impl/win_ole_mode.h

DEFINES += WINDOWS

LIBS += -lole32 -loleaut32
