#-------------------------------------------------
#
# Project created by QtCreator 2017-05-15T17:28:28
#
#-------------------------------------------------

QT += core gui network positioning

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CentralParking
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    boton.cpp \
    datamanager.cpp \
    mapa.cpp \
    posmanager.cpp

HEADERS  += widget.h \
    boton.h \
    datamanager.h \
    mapa.h \
    posmanager.h

FORMS    += widget.ui \
    boton.ui \
    mapa.ui

RESOURCES += \
    imagen.qrc
