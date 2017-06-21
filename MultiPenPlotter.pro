TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -pthread


SOURCES += main.cpp \
    plotter.cpp \
    logwriter.cpp \
    kernel.cpp \
    commandreader.cpp \
    sharingqueues.cpp \
    staticimitationinputcommand.cpp \
    speedcalculate.cpp \
    motor.cpp \
    pen.cpp


HEADERS += \
    plotter.h \
    threadqueue.h \
    logwriter.h \
    kernel.h \
    commandreader.h \
    sharingqueues.h \
    staticimitationinputcommand.h \
    speedcalculate.h \
    motor.h \
    pen.h

