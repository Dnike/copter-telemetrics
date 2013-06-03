#-------------------------------------------------
#
# Project created by QtCreator 2013-03-19T19:16:52
#
#-------------------------------------------------

QT       += core gui\
            network\
            opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = desktop-client
TEMPLATE = app


SOURCES  += main.cpp\
            mainwindow.cpp\
            scene3D.cpp

HEADERS  += mainwindow.hpp\
            scene3D.h

FORMS    += mainwindow.ui

CONFIG += qwt


INCLUDEPATH += /usr/include/qwt

LIBS += -lqwt -lGLU
