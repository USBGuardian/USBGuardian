#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T08:25:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = USBGuardian
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    details.cpp

HEADERS  += mainwindow.h \
    details.h

FORMS    += mainwindow.ui \
    details.ui

RESOURCES += \
    resources.qrc
