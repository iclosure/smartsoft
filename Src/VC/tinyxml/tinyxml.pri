################################################################
# tinyxml application
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the smartsoft License, Version 1.0
################################################################

PRJ_ROOT = $${PWD}/../../Qt/$${APP_LNAME}

contains(APP_CONFIG, dll) {
    CONFIG += dll
    win32|symbian: DEFINES += Q_WS_WIN QT_DLL $${APP_UNAME}_DLL $${APP_UNAME}_MAKEDLL

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
    ../../VC/tinyxml/tinyxml.h \
    ../../VC/tinyxml/tinyxml_global.h

SOURCES += \
    ../../VC/tinyxml/tinyxml.cpp \
    ../../VC/tinyxml/tinyxmlerror.cpp \
    ../../VC/tinyxml/tinyxmlparser.cpp

OTHER_FILES +=

