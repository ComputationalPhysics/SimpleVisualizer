TEMPLATE = app
CONFIG -= app_bundle
CONFIG += c++11

QT += qml quick widgets opengl openglextensions

SOURCES += main.cpp \
    billboards.cpp \
    renderer.cpp \
    controller.cpp \
    simulator.cpp \
    points.cpp \
    renderableobject.cpp
RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    renderer.h \
    billboards.h \
    controller.h \
    simulator.h \
    points.h \
    renderableobject.h
