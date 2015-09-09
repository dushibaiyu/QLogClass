#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T12:37:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QLogTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

include ($$PWD/../QLog/QLogClass.pri)
