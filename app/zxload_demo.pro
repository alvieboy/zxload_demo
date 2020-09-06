TARGET = zxload_demo
QT += widgets network

INCLUDEPATH=.. ../QtSpecem ../QtZeroConf
SOURCES=gui.cpp ImageWidget.cpp RemoteScreen.cpp Demo.cpp DemoWidget.cpp
HEADERS=ImageWidget.h RemoteScreen.h Demo.h DemoWidget.h

LIBS+=-L.. -L../QtSpecem -L../QtZeroConf -lQtSpecem -lQtZeroConf

QMAKE_RESOURCE_FLAGS += -no-compress

RESOURCES=zxload_demo.qrc
