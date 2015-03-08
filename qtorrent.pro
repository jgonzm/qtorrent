#-------------------------------------------------
#
# Project created by QtCreator 2015-03-06T17:55:55
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = qtorrent
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11
CONFIG   += static

TEMPLATE = app

INCLUDEPATH += .//libtorrent-rasterbar-1.0.3/include \
                /opt/local/include \

HEADERS += consolewindow.h \
           commandlineparseresult.h \

SOURCES += consolewindow.cpp \
           commandlineparseresult.cpp \
           main.cpp \

LIBS += "/opt/local/lib/libcrypto.a" \
        "/opt/local/lib/libz.a" \
        "/opt/local/lib/libssl.a" \
        "/opt/local/lib/libboost_system-mt.a" \
        "$$PWD/libtorrent-rasterbar-1.0.3/src/.libs/libtorrent-rasterbar.a"

QMAKE_CXXFLAGS += -fvisibility=hidden


