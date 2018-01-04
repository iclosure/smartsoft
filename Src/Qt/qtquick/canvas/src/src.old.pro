TEMPLATE = lib
CONFIG += qt plugin
QT += declarative
QT += script
 
TARGET  = $$qtLibraryTarget(canvasplugin)
CONFIG += debug_and_release
CONFIG += build_all

 DESTDIR = ..\\Canvas
 OBJECTS_DIR = tmp
 MOC_DIR = tmp

 HEADERS += context2d.h \
            canvas.h \
            canvastimer.h \
            canvasplugin.h

 SOURCES += context2d.cpp \
            canvas.cpp \
            canvasplugin.cpp \
    canvastimer.cpp
