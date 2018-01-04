TEMPLATE = app
SOURCES += main.cpp
TARGET = WinHostTest
DESTDIR = $${PWD}/../../bin


################################################################
# version check qt
################################################################

greaterThan(QT_MAJOR_VERSION, 4){
    QT += core gui widgets
}

CONFIG -= app_bundle

win32:CONFIG(release, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/QWinmigrate-3.0.0/lib/ -lQWinmigrate
else:win32:CONFIG(debug, debug|release): LIBS += -L$$(SMARTKITS_HOME)/Qt/QWinmigrate-3.0.0/lib/ -lQWinmigrated
else:unix: LIBS += -L(SMARTKITS_HOME)/Qt/QWinmigrate-3.0.0/lib/ -lQWinmigrate3

INCLUDEPATH += $(SMARTKITS_HOME)/Qt/QWinmigrate-3.0.0/include
DEPENDPATH += $$(SMARTKITS_HOME)/Qt/QWinmigrate-3.0.0/include
