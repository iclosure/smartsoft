
######################################################################
# Definitions
######################################################################

APP_LNAME   = QWinmigrate
APP_UNAME   = QWINMIGRATE

######################################################################
# Copyright Information
######################################################################

QMAKE_TARGET_COMPANY = "Smartsoft"
QMAKE_TARGET_DESCRIPTION = "MFC Migration Framework library"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2014 Smartsoft Inc."

VER_MAJ = 3
VER_MIN = 0
VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

######################################################################
# Build the static/shared libraries.
# If dll is enabled, a shared library is built, otherwise
# it will be a static library.
######################################################################

#APP_CONFIG           += dll

######################################################################
# widgets enables all classes, that are needed to use the all other
# widgets (sliders, dials, ...).
######################################################################

APP_CONFIG += widgets

######################################################################
# If you want to display svg images on the plot canvas, or
# export a plot to a SVG document
######################################################################

#APP_CONFIG     += svg

######################################################################
# If you want to use a OpenGL plot canvas
######################################################################

#APP_CONFIG     += opengl

######################################################################
#
######################################################################

infile(config.pri, SOLUTIONS_LIBRARY, yes): CONFIG += qtwinmigrate-uselib
TEMPLATE += fakelib

QTWINMIGRATE_LIBNAME = $$qtLibraryTarget($${APP_LNAME})
TEMPLATE -= fakelib
QTWINMIGRATE_LIBDIR = $${PWD}/../../../Qt/$${APP_LNAME}-$${VERSION}/lib
unix:qtwinmigrate-uselib:!qtwinmigrate-buildlib:QMAKE_RPATHDIR += $$QTWINMIGRATE_LIBDIR
