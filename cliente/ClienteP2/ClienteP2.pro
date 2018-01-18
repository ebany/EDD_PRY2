#-------------------------------------------------
#
# Project created by QtCreator 2017-11-10T05:57:36
#
#-------------------------------------------------

QT       += core gui
#Paso 1: Agregar los componentes web
QT       += webkitwidgets
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClienteP2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Hash.cpp \
    Arbol.cpp \
    Diccionario.cpp

HEADERS  += mainwindow.h \
    Hash.h \
    Arbol.h \
    Diccionario.h \
    Grafo.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    mapa.html
