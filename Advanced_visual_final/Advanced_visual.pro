#-------------------------------------------------
#
# Project created by QtCreator 2017-07-22T17:13:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Advanced_visual
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cal.cpp \
    check.cpp \
    initfilter.cpp

HEADERS  += mainwindow.h \
    cal.h \
    check.h \
    initfilter.h \
    origin.h

FORMS    += mainwindow.ui


INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv \
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so
