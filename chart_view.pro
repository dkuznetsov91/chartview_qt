QT       += core gui  widgets

TARGET = chart_view
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    src/main.cpp \
    src/axisview.cpp \
    src/mainwindow.cpp \
    src/plotter.cpp \
    src/graph.cpp \
    src/axis.cpp \
    src/parameter.cpp \
    src/doublescreenbuffer.cpp \
    src/vizir.cpp \
    src/parametermodel.cpp \
    src/legendview.cpp \
    src/ParameterAttributes.cpp

HEADERS += \
    src/axisview.h \
    src/mainwindow.h \
    src/plotter.h \
    src/graph.h \
    src/axis.h \
    src/parameter.h \
    src/doublescreenbuffer.h \
    src/vizir.h \
    src/parametermodel.h \
    src/legendview.h \
    src/ParameterAttributes.h \
    src/rest_marker.h

FORMS += \
    src/mainwindow.ui
