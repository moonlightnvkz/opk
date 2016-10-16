TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    console.c

HEADERS += \
    console.h
LIBS += -lncurses
