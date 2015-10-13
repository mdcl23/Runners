#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T14:27:02
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Game
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    landscapewidget.cpp \
    landscape.cpp \
    world.cpp \
    gamescene.cpp \
    dijkstra.cpp \
    hexgame.cpp \
    hexscene.cpp \
    hexbrain.cpp \
    hexutils.cpp \
    tetrixboard.cpp \
    tetrixpiece.cpp \
    tetrixwindow.cpp

HEADERS  += mainwindow.h \
    landscapewidget.h \
    landscape.h \
    world.h \
    gamescene.h \
    dijkstra.h \
    hexgame.h \
    hexscene.h \
    hexbrain.h \
    hexutils.h \
    tetrixboard.h \
    tetrixpiece.h \
    tetrixwindow.h

FORMS    += mainwindow.ui

DISTFILES += \
    portal.png

RESOURCES += \
    runner-resources.qrc
