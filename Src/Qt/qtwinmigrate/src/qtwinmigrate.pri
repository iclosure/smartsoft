################################################################
# QtWinmigrate library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the QtWinmigrate License, Version 2.8
################################################################

include(../common.pri)

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

contains(APP_CONFIG, framework) {
    CONFIG += lib_bundle
}

################################################################
# import sources in current directory
################################################################

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
LIBS += -luser32

# When built as a library, the default is to include the component's
# MFC support. To disable this (e.g. in order to use this component
# with Visual Studio Express, which does not include MFC), comment out
# the following line.
qtwinmigrate-uselib:DEFINES *= _AFXDLL

qtwinmigrate-uselib:!qtwinmigrate-buildlib {
    LIBS += -L$$QTWINMIGRATE_LIBDIR -l$$QTWINMIGRATE_LIBNAME
} else {
    SOURCES += $$PWD/qwinwidget.cpp $$PWD/qwinhost.cpp $$PWD/qmfcapp.cpp
    HEADERS += $$PWD/qwinwidget.h $$PWD/qwinhost.h $$PWD/qmfcapp.h
}

win32 {
    qtwinmigrate-buildlib:DEFINES += QT_QTWINMIGRATE_EXPORT
    else:qtwinmigrate-uselib:DEFINES += QT_QTWINMIGRATE_IMPORT
}
