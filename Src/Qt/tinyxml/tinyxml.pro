################################################################
# tinyxml library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the tinyxml License, Version 1.0
################################################################

######################################################################
# qmake project file for building the libraries
######################################################################

include( config.pri )
include( ../../VC/$${APP_LNAME}/$${APP_LNAME}.pri )

################################################################
# project config
################################################################

QT          -= core gui

TEMPLATE    = lib
TARGET      = Qt$${QT_MAJOR_VERSION}$$qtLibraryTarget($${APP_LNAME})
DESTDIR     = $${PWD}/../../../Qt/$${APP_LNAME}-$${VERSION}/lib

######################################################################
# qmake internal options
######################################################################

CONFIG      += warn_on
CONFIG      += no_keywords
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

} else {

    CONFIG           += release
}

linux-g++ {
    # CONFIG           += separate_debug_info
}

######################################################################
# paths for building tinyxml
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
