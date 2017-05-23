#-------------------------------------------------
#
# Project created by QtCreator 2017-05-22T20:12:13
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14
CONFIG -= app_bundle

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CVCourse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    depth.cpp \
    image.cpp \
    matrix.cpp \
    qimagemaker.cpp

HEADERS  += mainwindow.h \
    depth.h \
    image.h \
    matrix.h \
    qimagemaker.h

FORMS    += mainwindow.ui

INCLUDEPATH += E:\opencv-build\install\include
LIBS += -LE:\opencv-build\install\x64\vc14\lib \
    -lopencv_core320 \
    -lopencv_highgui320 \
    -lopencv_imgcodecs320 \
    -lopencv_imgproc320 \
    -lopencv_features2d320 \
    -lopencv_calib3d320
