TEMPLATE = app
TARGET = smart
INCLUDEPATH += .

QT += core gui multimedia network concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT_CONFIG -= no-pkg-config

CONFIG  += link_pkgconfig

#PKGCONFIG += opencv #Jin's
PKGCONFIG += opencv4 #Augustine's

#LIBS += -L/usr/local/Cellar/opencv/3.4.2/lib/ #Jin's
LIBS += -L/usr/local/Cellar/opencv/4.2.0_4/lib/ #Augustine's


# Input
HEADERS += mainwindow.h capture_thread.h \
    utilities.h
SOURCES += main.cpp mainwindow.cpp capture_thread.cpp \
    utilities.cpp

# Using OpenCV or QCamera
# DEFINES += GAZER_USE_QT_CAMERA=1
# QT += multimediawidgets
# DEFINES += OPENCV_DATA_DIR=\\\"/usr/share/opencv/\\\"
#DEFINES += OPENCV_DATA_DIR=\\\"/usr/local/Cellar/opencv/3.4.2/share/OpenCV/\\\" #Jin's
DEFINES += OPENCV_DATA_DIR=\\\"/usr/local/Cellar/opencv/4.2.0_4/share/opencv4/\\\" #Augustine's
