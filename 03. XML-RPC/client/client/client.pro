#-------------------------------------------------
#
# Project created by QtCreator 2017-02-23T13:24:57
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp\
        mainwindow.cpp \
    servermanager.cpp

HEADERS  += mainwindow.h \
    servermanager.h

FORMS    += \
    mainwindow.ui

unix:!macx: LIBS += -L$$PWD/../../../../../../../../QtLibs/build-maia-Desktop_Qt_5_7_1_GCC_64bit-Debug/ -lmaia

INCLUDEPATH += $$PWD/../../../../../../../../QtLibs/libmaia
DEPENDPATH += $$PWD/../../../../../../../../QtLibs/libmaia

unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../../../../../QtLibs/build-maia-Desktop_Qt_5_7_1_GCC_64bit-Debug/libmaia.a
