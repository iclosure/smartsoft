################################################################
# JFlowChart library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

PRJ_ROOT    = $${PWD}/..

contains(APP_CONFIG, dll) {
    CONFIG += dll
    win32|symbian: DEFINES +=  QT_DLL $${APP_UNAME}_DLL $${APP_UNAME}_MAKEDLL
    greaterThan(QT_MAJOR_VERSION, 4) {
        DEFINES += Q_WS_WIN
    }

    SOURCES +=

} else {
    CONFIG += staticlib
}

contains(APP_CONFIG, widgets) {
    DEFINES += APP_IMPORT_WIDGET
    greaterThan(QT_MAJOR_VERSION, 4) {
        QT += widgets
    } else {
#        message("widgets not supported on Qt version " + $${QT_VERSION})
    }
}

contains(APP_CONFIG, winextras) {
    DEFINES += APP_IMPORT_WINEXTRAS
    greaterThan(QT_MAJOR_VERSION, 4) {
        QT += winextras
    } else {
#        message("winextras not supported on Qt version " + $${QT_VERSION})
    }
}

contains(APP_CONFIG, framework) {
    CONFIG += lib_bundle
}

################################################################
# import sources in subdirectory
################################################################

################################################################
# import sources in current directory
################################################################

HEADERS += \
    $$PWD/jflowchart.h \
    $$PWD/jshape.h \
    $$PWD/jlinker.h \
    $$PWD/jlamp.h \
    $$PWD/jflowchart_p.h \
    $$PWD/jflowchart_global.h \
    $$PWD/jview.h \
    $$PWD/jscene.h

SOURCES += \
    $$PWD/jflowchart.cpp \
    $$PWD/jshape.cpp \
    $$PWD/jlinker.cpp \
    $$PWD/jlamp.cpp \
    $$PWD/jflowchart_p.cpp \
    $$PWD/jscene.cpp \
    $$PWD/jview.cpp
