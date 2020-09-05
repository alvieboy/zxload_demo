TARGET = zxload_demo
QT += widgets network

INCLUDEPATH=.. ../QtSpecem
SOURCES=gui.cpp
HEADERS=LogEmitter.h

LIBS+=-L.. -L../QtSpecem -lQtSpecem
