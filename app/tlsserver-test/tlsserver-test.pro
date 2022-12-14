TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../../src
LIBS += -lssl -lcrypto
DESTDIR=../../bin
SOURCES += \
        ../../src/tcpserver.cpp \
        ../../src/tcpsession.cpp \
        ../../src/tlsserver.cpp \
        ../../src/tlssession.cpp \
        tlsserver-test.cpp

HEADERS += \
	../../src/server.h \
	../../src/session.h \
	../../src/tcpserver.h \
	../../src/tcpsession.h \
	../../src/tlsserver.h \
	../../src/tlssession.h
