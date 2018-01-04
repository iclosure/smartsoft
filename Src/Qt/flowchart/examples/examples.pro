################################################################
# Project created by QtCreator 2013-06-04T13:26:00
#
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft, version 1.0
################################################################

######################################################################
# Copyright Information
######################################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "FlowChart Demo"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014-? Smartsoft. All rights reserved."

#win32:RC_ICONS =

TARGET_VER_MAJ = 1
TARGET_VER_MIN = 0
TARGET_VER_PAT = 0
DEFINES += TARGET_VER_MAJ TARGET_VER_MIN TARGET_VER_PAT

################################################################
# project config
################################################################

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

TEMPLATE = app
TARGET = flowchart

######################################################################
# qmake internal options
######################################################################

#CONFIG += thread static
#CONFIG += static
CONFIG -= app_bundle

######################################################################
# release/debug mode
######################################################################

win32 {
    CONFIG(release, debug|release): {
        DEFINES += QT_NO_DEBUG_OUTPUT
    }
} else {
    CONFIG += release
    DEFINES += QT_NO_DEBUG_OUTPUT
}

linux-g++ {
    CONFIG += separate_debug_info
}

################################################################
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

CONFIG += precompile_header
PRECOMPILED_HEADER = $$PWD/../precomp.h
INCLUDEPATH += $$PWD/..

################################################################
# Resouces
################################################################

RESOURCES += \

################################################################
# Translations
################################################################

TRANSLATIONS += \

################################################################
# Others
################################################################

OTHER_FILES += \

################################################################
# import libraries
################################################################

## import FlowChart library

win32:CONFIG(release, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/jflowchart-1.0.0/lib/ -lqt$${QT_MAJOR_VERSION}jflowchart1
else:win32:CONFIG(debug, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/jflowchart-1.0.0/lib/ -lqt$${QT_MAJOR_VERSION}jflowchartd1
else:unix: LIBS += -L$$(SMARTKITS_HOME)/Qt/jflowchart-1.0.0/lib/ -lqt$${QT_MAJOR_VERSION}jflowchart1

INCLUDEPATH += $$(SMARTKITS_HOME)/Qt/jflowchart-1.0.0/include
DEPENDPATH += $$(SMARTKITS_HOME)/Qt/jflowchart-1.0.0/include

SOURCES += \
    main.cpp \
    flowchart.cpp

HEADERS += \
    flowchart.h
