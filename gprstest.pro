#-------------------------------------------------
#
# Project created by QtCreator 2014-07-08T19:34:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gprstest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gprsthread.cpp \
    serial/qextserialport.cpp \
    serial/qextserialbase.cpp \
    serial/posix_qextserialport.cpp

HEADERS  += mainwindow.h \
    gprsthread.h \
    serial/qextserialport.h \
    serial/qextserialbase.h \
    serial/posix_qextserialport.h

FORMS    += mainwindow.ui
