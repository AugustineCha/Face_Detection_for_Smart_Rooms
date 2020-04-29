QT += core gui multimedia network concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT_CONFIG -= no-pkg-config

CONFIG  += link_pkgconfig

PKGCONFIG += opencv

LIBS += -L/usr/local/Cellar/opencv/3.4.2/lib/

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += OPENCV_DATA_DIR=\\\"/usr/local/Cellar/opencv/3.4.2/share/OpenCV/\\\"

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    capture_thread.cpp \
    main.cpp \
    smartrooms.cpp \
    utilities.cpp

HEADERS += \
    capture_thread.h \
    smartrooms.h \
    utilities.h

FORMS += \
    smartrooms.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
