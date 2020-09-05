TARGET = zxload_demo
QT += widgets network

INCLUDEPATH=.. ../QtSpecem
SOURCES=gui.cpp
HEADERS=

LIBS+=-L.. -L../QtSpecem -lQtSpecem
