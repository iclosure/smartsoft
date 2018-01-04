TEMPLATE = app
TARGET	 = QtMfc2

greaterThan(QT_MAJOR_VERSION, 4){
    QT += core gui widgets
}

DEFINES -= UNICODE
DEFINES += _AFXDLL WINVER=0x0500
QMAKE_LIBS_QT_ENTRY =

HEADERS = childview.h mainframe.h qtmfc.h stdafx.h
SOURCES = childview.cpp mainframe.cpp qtmfc.cpp stdafx.cpp
RC_FILE = qtmfc.rc

include(../../../src/qtwinmigrate.pri)
