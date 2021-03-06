TEMPLATE = app
CONFIG -= app_bundle
CONFIG += c++11

QT += qml quick widgets opengl openglextensions

SOURCES += main.cpp \
    renderer.cpp \
    controller.cpp \
    simulator.cpp \
    renderableobject.cpp \
    billboards2d.cpp \
    points2d.cpp \
    mycontroller.cpp
RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    renderer.h \
    controller.h \
    simulator.h \
    renderableobject.h \
    billboards2d.h \
    points2d.h \
    mycontroller.h
