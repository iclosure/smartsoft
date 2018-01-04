################################################################
# qml-canvas library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

######################################################################
# qmake project file for building the libraries
######################################################################

include( config.pri )
include( src/src.pri )

################################################################
# project config
################################################################

QT          += script declarative
CONFIG += qt plugin

TEMPLATE    = lib
TARGET      = $$qtLibraryTarget(qml$${APP_LNAME}plugin)

uri = com.nokia.canvas

defineReplace(installDir) {
    prefix =
    sers = $$split(1, .)
    return ($$join(sers, \\, $$prefix))
}

DESTDIR = $$(SMARTKITS_HOME)\\Qt\\QtQuick\\$$replace(uri, \\., /)
#DESTDIR = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)

################################################################
# version check qt
################################################################

lessThan(QT_VERSION, 5.2.0) {
#    message("Cannot build $${APP_LNAME} with Qt version $${QT_VERSION}.")
#    error("Use at least Qt 5.2.0.")
}

######################################################################
# qmake internal options
######################################################################

CONFIG      += warn_on
#CONFIG      += no_keywords
#CONFIG      += silent

######################################################################
# release/debug mode
######################################################################

win32 {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version. For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG           += debug_and_release
    CONFIG           += build_all
    DEFINES          += QT_NO_DEBUG_OUTPUT

    exists(buildex.cmd): system( call buildex.cmd $$PWD $$DESTDIR $$url)

} else {

    CONFIG           += release

}

linux-g++ {
    # CONFIG           += separate_debug_info
}

######################################################################
# paths for building current target
######################################################################

unix {
    exists( $${QMAKE_LIBDIR_QT}/lib$${APP_LNAME}.* ) {

        # On some Linux distributions the libraries are installed
        # in the same directory as the Qt libraries. Unfortunately
        # qmake always adds QMAKE_LIBDIR_QT at the beginning of the
        # linker path, so that the installed libraries will be
        # used instead of the local ones.

        error( "local build will conflict with $${QMAKE_LIBDIR_QT}/lib$${APP_LNAME}.*" )
    }
}

################################################################
# Resouces
################################################################

RESOURCES += \

################################################################
# Translations
################################################################

TRANSLATIONS += \

################################################################
# Other files
################################################################

OTHER_FILES += \
    qmldir \
    qmldir

################################################################
# import libraries
################################################################

## import common library

################################################################
# Install
################################################################

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    maemo5 | !isEmpty(MEEGO_VERSION_MAJOR) {
        installPath = /usr/lib/qt4/imports/$$replace(uri, \\., /)
    } else {
        installPath = $$[QT_INSTALL_IMPORTS]/$$replace(uri, \\., /)
    }
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}
