################################################################
# QCustomPlot library
# Copyright (C) 2014 Hi ROBOT
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the smartsoft License, Version 1.0
################################################################

PRJ_ROOT    = $${PWD}/..

contains(APP_CONFIG, dll) {
    CONFIG += dll
    win32|symbian: DEFINES += Q_WS_WIN QT_DLL $${APP_UNAME}_DLL $${APP_UNAME}_MAKEDLL

#    win32:CONFIG(release, debug|release): LIBS += Qt5PlatformSupport.lib qwindows.lib
#    else:win32:CONFIG(debug, debug|release): LIBS += Qt5PlatformSupportd.lib qwindowsd.lib \

} else {
    CONFIG += staticlib
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
    src/qcustomplot.h

SOURCES += \
    src/qcustomplot.cpp
    
