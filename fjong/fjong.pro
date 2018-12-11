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

CONFIG += warn_off

linux-g*{
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_CXXFLAGS +=  -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-comment -Wno-parentheses -Wno-delete-non-virtual-dtor -Wno-missing-noreturn
    LIBS += -fopenmp
    QMAKE_CXXFLAGS +=  -Ofast -O3
    DEFINES += IGNORE_CUDA
}



win32:CLIBS += -L$$CUDA_DIR/lib64 -lcuda -lcudart

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
    QMAKE_CXXFLAGS += -openmp -O3
    LIBS += -openmp
    QMAKE_CXXFLAGS += -Ofast
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
    source/Raytracer/raytracer.h \

FORMS += \
        mainwindow.ui \
    source/forms/dialogeffects.ui

# Default rules for deployment.

win32 {
CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.0/"

CUDA_OBJECTS_DIR = OBJECTS_DIR/../cuda

# This makes the .cu files appear in your project
OTHER_FILES += \
    source/cuda/raytracer.cu
CUDA_SOURCES += \
    source/cuda/raytracer.cu
#    source/cuda/vec3.h

#-------------------------------------------------

# MSVCRT link option (static or dynamic, it must be the same with your Qt SDK link option)
MSVCRT_LINK_FLAG_DEBUG   = "/MDd"
MSVCRT_LINK_FLAG_RELEASE = "/MD"

# CUDA settings
SYSTEM_NAME = x64                 # Depending on your system either 'Win32', 'x64', or 'Win64'
SYSTEM_TYPE = 64                    # '32' or '64', depending on your system
CUDA_ARCH = sm_30                   # Type of CUDA architecture
NVCC_OPTIONS = --use_fast_math

# include paths
INCLUDEPATH += $$CUDA_DIR/include \
               $$CUDA_DIR/common/inc \
               $$CUDA_DIR/../shared/inc

# library directories
QMAKE_LIBDIR += $$CUDA_DIR/lib/$$SYSTEM_NAME \
                $$CUDA_DIR/common/lib/$$SYSTEM_NAME \
                $$CUDA_DIR/../shared/lib/$$SYSTEM_NAME

# The following makes sure all path names (which often include spaces) are put between quotation marks
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')

# Add the necessary libraries
CUDA_LIB_NAMES = cudart_static kernel32 user32 gdi32 winspool comdlg32 \
                 advapi32 shell32 ole32 oleaut32 uuid odbc32 odbccp32  \
                 #freeglut glew32

for(lib, CUDA_LIB_NAMES) {
    CUDA_LIBS += -l$$lib
}
LIBS += $$CUDA_LIBS

# Configuration of the Cuda compiler
CONFIG(debug, debug|release) {
    # Debug mode
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.obj
    cuda_d.commands = $$CUDA_DIR/bin/nvcc.exe -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$LIBS \
                      --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH \
                      --compile -cudart static -g -DWIN32 -D_MBCS \
                      -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/Od,/Zi,/RTC1" \
                      -Xcompiler $$MSVCRT_LINK_FLAG_DEBUG \
                      -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release mode
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.obj
    cuda.commands = $$CUDA_DIR/bin/nvcc.exe $$NVCC_OPTIONS $$CUDA_INC $$CLIBS \
                    --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH \
                    --compile -cudart static -DWIN32 -D_MBCS \
                    -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/O2,/Zi" \
                    -Xcompiler $$MSVCRT_LINK_FLAG_RELEASE \
                    -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

}
