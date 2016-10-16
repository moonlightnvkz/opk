TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    console.c \
    main.c \
    mystack.c \
    labyrinth.c

HEADERS += \
    console.h \
    mystack.h \
    labyrinth.h \
    cell.h \
    constants.h
LIBS += -lncurses
