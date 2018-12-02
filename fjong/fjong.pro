#-------------------------------------------------
#
# Project created by QtCreator 2018-12-01T18:24:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fjong
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS



# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

macx {
    QMAKE_CXXFLAGS += -openmp
    #LIBS += -openmp
    ICON = trse.icns
    QMAKE_CXXFLAGS += -Ofast

}

win32-g++ {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
    QMAKE_CXXFLAGS += -Ofast

}
win32-msvc*{
    QMAKE_CXXFLAGS += -openmp
    LIBS += -openmp
    QMAKE_CXXFLAGS += -Ofast
}

linux-g*{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_CXXFLAGS +=  -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-comment -Wno-parentheses -Wno-delete-non-virtual-dtor -Wno-missing-noreturn
    LIBS += -fopenmp
    QMAKE_CXXFLAGS +=  -Ofast

}

INCLUDEPATH += ../../TRSE/source/LeLib/util/

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    source/Raytracer/material.cpp \
    source/Raytracer/camera.cpp \
    source/Raytracer/raytracerglobals.cpp \
    source/Raytracer/raytracer.cpp \
    source/Raytracer/rayobject.cpp \
    ../../TRSE/source/LeLib/util/util.cpp \
    ../../TRSE/source/LeLib/util/cinifile.cpp \
    ../../TRSE/source/LeLib/util/SimplexNoise.cpp \
    source/forms/dialogeffects.cpp \
    source/forms/raytracer1.cpp \
        ../../TRSE/source/LeLib/util/cinifile.cpp \
        ../../TRSE/source/LeLib/util/SimplexNoise.cpp \
        ../../TRSE/source/LeLib/util/util.cpp

HEADERS += \
        mainwindow.h \
    source/Raytracer/raytracer.h \
    source/Raytracer/raytracerglobals.h \
    source/Raytracer/rayobject.h \
    source/Raytracer/material.h \
    source/Raytracer/camera.h \
    ../../TRSE/source/LeLib/util/util.h \
    ../../TRSE/source/LeLib/util/cinifile.h \
    ../../TRSE/source/LeLib/util/random.h \
    ../../TRSE/source/LeLib/util/SimplexNoise.h \
    source/forms/dialogeffects.h \
    source/forms/raytracer1.h \
    source/Raytracer/raytracer.h

FORMS += \
        mainwindow.ui \
    source/forms/dialogeffects.ui

# Default rules for deployment.
