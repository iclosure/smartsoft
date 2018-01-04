################################################################
# Kwt library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Kwt License, Version 1.0
################################################################

######################################################################
# qmake project file for building the libraries
######################################################################

include( config.pri )
include( src/src.pri )

################################################################
# project config
################################################################

QT          += core gui

TEMPLATE    = lib
TARGET      = Qt$${QT_MAJOR_VERSION}$$qtLibraryTarget($${APP_LNAME})
DESTDIR     = $${PWD}/../../../Qt/$${APP_LNAME}-$${VERSION}/lib

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

    exists(buildex.cmd): system( call buildex.cmd $$DESTDIR)

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
# creating a precompiled header file (only supported on some platforms (Windows - all MSVC project types,
# Mac OS X - Xcode, Makefile, Unix - gcc 3.3 and up)
################################################################

CONFIG += precompile_header
PRECOMPILED_HEADER = precomp.h
INCLUDEPATH += .

################################################################
# Resouces
################################################################

RESOURCES += \
    resource/forms/kwt_forms.qrc \
    resource/images/kwt_images.qrc \
    resource/lang/kwt_lang.qrc \
    resource/qss/kwt_qss.qrc

################################################################
# Translations
################################################################

TRANSLATIONS += \
    resource/lang/zh_CN.ts \
    resource/lang/en_US.ts

################################################################
# Other files
################################################################

OTHER_FILES += \
    buildex.cmd

################################################################
# import libraries
################################################################

## import common library
