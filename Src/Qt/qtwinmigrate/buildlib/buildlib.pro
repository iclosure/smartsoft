################################################################
# QtWinmigrate library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the QtWinmigrate License, Version 2.8
################################################################

######################################################################
# qmake project file for building the libraries
######################################################################

include( ../common.pri )

################################################################
# project config
################################################################

TEMPLATE    = lib
TARGET      = $${QTWINMIGRATE_LIBNAME}
DESTDIR     = $${QTWINMIGRATE_LIBDIR}

################################################################
# version check qt
################################################################

greaterThan(QT_MAJOR_VERSION, 4){
    QT += core gui widgets
}

######################################################################
# qmake internal options
######################################################################

CONFIG      += warn_on
CONFIG      += no_keywords
#CONFIG      += silent

######################################################################
# release/debug mode
######################################################################

win32|mac:!wince*:!win32-msvc:!macx-xcode {
    # On Windows you can't mix release and debug libraries.
    # The designer is built in release mode. If you like to use it
    # you need a release version. For your own application development you
    # might need a debug version.
    # Enable debug_and_release + build_all if you want to build both.

    CONFIG           += debug_and_release
    CONFIG           += build_all
#    DEFINES          += QT_NO_DEBUG_OUTPUT

    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QTWINMIGRATE_LIBNAME}.dll

} else {

    CONFIG           += release

}

CONFIG += qt dll qtwinmigrate-buildlib
mac:CONFIG += absolute_library_soname

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
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

DEFINES += QT_PRECOMP
CONFIG += precompile_header
PRECOMPILED_HEADER = precomp.h
INCLUDEPATH += $${PWD}

################################################################
# Include src
################################################################

include(../src/qtwinmigrate.pri)

######################################################################
# Install paths
######################################################################

APP_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]

unix {
    APP_INSTALL_PREFIX    = /usr/local/$${APP_LNAME}-$${VERSION}
}

win32 {
    APP_INSTALL_PREFIX    = $${DESTDIR}
}

APP_INSTALL_DOCS      = $${APP_INSTALL_PREFIX}/doc
APP_INSTALL_HEADERS   = $${APP_INSTALL_PREFIX}/include
APP_INSTALL_LIBS      = $${APP_INSTALL_PREFIX}/lib

target.path = $$DESTDIR
INSTALLS += target
