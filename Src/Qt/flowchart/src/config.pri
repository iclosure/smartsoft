################################################################
# JFlowChart library
# Copyright (C) 2014 Smartsoft
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Smartsoft License, Version 1.0
################################################################

######################################################################
# Definitions
######################################################################

APP_LNAME   = jflowchart
APP_UNAME   = JFLOWCHART

######################################################################
# Copyright Information
######################################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "JFlowChart library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014 Smartsoft Inc."

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

######################################################################
# Build the static/shared libraries.
# If dll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

APP_CONFIG           += dll ## 生成静态库

######################################################################
# widgets enables all classes, that are needed to use the all other
# widgets (sliders, dials, ...).
######################################################################

APP_CONFIG     += widgets

######################################################################
# winextras module will be enable or not.
######################################################################

#APP_CONFIG     += winextras

######################################################################
# Quick engine will be enable or not.
######################################################################

#APP_CONFIG     += quick

######################################################################
# If you want to display svg images on the plot canvas, or
# export a plot to a SVG document
######################################################################

#APP_CONFIG     += svg

######################################################################
# If you want to use a serialport module
######################################################################

#APP_CONFIG     += serialport

######################################################################
# If you want to use a OpenGL plot canvas
######################################################################

#APP_CONFIG     += opengl

######################################################################
# When Qt has been built as fra#mework qmake ( qtAddLibrary ) wants
# to link frameworks instead of regular libs
######################################################################

macx:CONFIG(qt_framework, qt_framework|qt_no_framework) {

    APP_CONFIG += framework
}

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

