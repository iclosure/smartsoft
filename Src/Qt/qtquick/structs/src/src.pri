################################################################
# qml-structs library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

PRJ_ROOT    = $${PWD}/..

contains(APP_CONFIG, dll) {
    CONFIG += dll
    win32|symbian: {
        DEFINES += QT_DLL $${APP_UNAME}_DLL $${APP_UNAME}_MAKEDLL
        qt:greaterThan(QT_MAJOR_VERSION, 4) {
            DEFINES += Q_WS_WIN
        }
    }

} else {
    CONFIG += staticlib
}

contains(APP_CONFIG, framework) {
    CONFIG += lib_bundle
}

################################################################
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

#CONFIG += precompile_header
#PRECOMPILED_HEADER = $$PWD/precomp.h
#INCLUDEPATH += $$PWD

################################################################
# import sources in subdirectory
################################################################

lessThan(QT_MAJOR_VERSION, 5) {
    HEADERS += \

    SOURCES += \
}

################################################################
# import sources in current directory
################################################################

HEADERS += \
    src/structs_plugin.h \
    src/flowchartdata.h

SOURCES += \
    src/structs_plugin.cpp \
    src/flowchartdata.cpp
