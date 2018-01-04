TEMPLATE = app
TARGET   = QtMfc5

greaterThan(QT_MAJOR_VERSION, 4){
    QT += core gui widgets
}

SOURCES += childview.cpp mainframe.cpp qtmfc.cpp
HEADERS += childview.h   mainframe.h
FORMS	+= ../step4/optionsdialog.ui
RC_FILE = qtmfc.rc
