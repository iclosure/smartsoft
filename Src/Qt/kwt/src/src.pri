################################################################
# Kwt library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Kwt License, Version 1.0
################################################################

PRJ_ROOT    = $${PWD}/..

contains(APP_CONFIG, dll) {
    CONFIG += dll
    win32|symbian: DEFINES += Q_WS_WIN QT_DLL $${APP_UNAME}_DLL $${APP_UNAME}_MAKEDLL

#    win32:CONFIG(release, debug|release): LIBS += Qt5PlatformSupport.lib qwindows.lib
#    else:win32:CONFIG(debug, debug|release): LIBS += Qt5PlatformSupportd.lib qwindowsd.lib \

    SOURCES +=

} else {
    CONFIG += staticlib
}

contains(APP_CONFIG, widgets) {
    DEFINES += APP_IMPORT_WIDGET
    greaterThan(QT_MAJOR_VERSION, 4) {
        QT += widgets
    } else {
        message("widgets not supported on Qt version " + QT_VERSION)
    }
}

contains(APP_CONFIG, winextras) {
    DEFINES += APP_IMPORT_WINEXTRAS
    greaterThan(QT_MAJOR_VERSION, 4) {
        QT += winextras
    } else {
        message("winextras not supported on Qt version " + QT_VERSION)
    }
}

contains(APP_CONFIG, quick) {
    DEFINES += APP_IMPORT_QUICK
    greaterThan(QT_MAJOR_VERSION, 4) {
        QT += qml quick

        HEADERS += \
            src/kquickview.h

        SOURCES += \
            src/kquickview.cpp

    } else {
        QT += declarative

        HEADERS += \
            src/kdeclarativeview.h

        SOURCES += \
            src/kdeclarativeview.cpp
    }
}

contains(APP_CONFIG, framework) {
    CONFIG += lib_bundle
}

################################################################
# import sources in subdirectory
################################################################

contains(APP_CONFIG, serialport) {
    lessThan(QT_MAJOR_VERSION, 5) {
        HEADERS += \
#           src/serialport/posix_qextserialport.h \
            src/serialport/qextserialbase.h \
            src/serialport/qextserialenumerator.h \
            src/serialport/qextserialport.h \
            src/serialport/win_qextserialport.h

        SOURCES += \
#           src/serialport/posix_qextserialport.cpp \
            src/serialport/qextserialbase.cpp \
            src/serialport/qextserialenumerator.cpp \
            src/serialport/qextserialport.cpp \
            src/serialport/win_qextserialport.cpp
    }
}

################################################################
# import sources in current directory
################################################################

HEADERS += \
    src/kwt_global.h \
    src/kwidget.h \
    src/ktoolbutton.h \
    src/ktitle.h \
    src/ktabwidget.h \
    src/kskinbutton.h \
    src/kshake.h \
    src/kpushbutton.h \
    src/kmenu.h \
    src/kdialog.h \
    src/kdragproxy.h \
    src/ksystemmenu.h \
    src/kstatusbar.h \
    src/khexedit.h \
    src/kpanetitle.h \
    src/ktooltitle.h \
    src/kcustomtitle.h \
    src/kribbontitle.h \
    src/kcheckbox.h \
    src/kencrypt.h \
    src/kresizedialogproxy.h \
    src/kstylesheet.h \
    src/kwaitcondition.h \
    src/kthread.h \
    src/kthread_p.h \
    src/kradiobutton.h \
    src/kwtinc.h

SOURCES += \
    src/kwidget.cpp \
    src/ktoolbutton.cpp \
    src/ktitle.cpp \
    src/ktabwidget.cpp \
    src/kskinbutton.cpp \
    src/kshake.cpp \
    src/kpushbutton.cpp \
    src/kmenu.cpp \
    src/kdialog.cpp \
    src/kwt_global.cpp \
    src/kdragproxy.cpp \
    src/ksystemmenu.cpp \
    src/kstatusbar.cpp \
    src/khexedit.cpp \
    src/kpanetitle.cpp \
    src/ktooltitle.cpp \
    src/kcustomtitle.cpp \
    src/kribbontitle.cpp \
    src/kcheckbox.cpp \
    src/kencrypt.cpp \
    src/kresizedialogproxy.cpp \
    src/kstylesheet.cpp \
    src/kwaitcondition.cpp \
    src/kthread.cpp \
    src/kthread_p.cpp \
    src/kradiobutton.cpp
