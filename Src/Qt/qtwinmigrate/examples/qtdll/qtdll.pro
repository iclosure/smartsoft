TEMPLATE     = lib
CONFIG	    += dll
SOURCES	     = main.cpp

greaterThan(QT_MAJOR_VERSION, 4){
    QT += core gui widgets
}

TARGET	     = qtdialog
DLLDESTDIR   = $$[QT_INSTALL_PREFIX]/bin

include(../../src/qtwinmigrate.pri)
