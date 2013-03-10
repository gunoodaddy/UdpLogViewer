#-------------------------------------------------
#
# Project created by QtCreator 2013-03-10T22:50:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UdpLogViewer
TEMPLATE = app

CONFIG   += precompile_header

SOURCES += \
	main.cpp \
	udplogviewer.cpp \
	GlobalEvent.cpp \
	SettingManager.cpp \
	stdafx.cpp \
	UdpLogManager.cpp \
	UpgradeWindow.cpp \
        Util.cpp \
        NetworkEngine/DefferedCaller.cpp \
        UpgradeManager/UpgradeManager.cpp

HEADERS  += \
	udplogviewer.h \
	GlobalEvent.h \
	resource.h \
	SettingManager.h \
	Singleton.h \
	stdafx.h \
	UdpLogManager.h \
	UpgradeWindow.h \
        Util.h \
        NetworkEngine/DefferedCaller.h \
        NetworkEngine/INetPeerEvent.h \
        NetworkEngine/NetServiceRunner.h \
        NetworkEngine/NetUdpSession.h \
        UpgradeManager/UpgradeManager.h
	
	
FORMS    +=  \
	udplogviewer.ui \
	UpgradeWindow.ui

INCLUDEPATH += $(BOOST_DIR)

RESOURCES += \
    udplogviewer.qrc

LIBS += -L$(BOOST_DIR)/lib

win32{
RC_FILE = UdpLogViewer.rc
QMAKE_CXXFLAGS_DEBUG += -wd4100 -wd4101
QMAKE_CXXFLAGS_RELEASE += -wd4100 -wd4101
LIBS += -luser32 -lshell32 -lgdi32
}

PRECOMPILED_HEADER = stdafx.h

macx{
QMAKE_CXXFLAGS_WARN_ON = ""
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-reorder
}

# Requied for some C99 defines
DEFINES += __STDC_CONSTANT_MACROS
