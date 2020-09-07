TARGET = zxload_demo
QT += widgets network

INCLUDEPATH=.. ../QtSpecem ../QtZeroConf
SOURCES=gui.cpp ImageWidget.cpp RemoteScreen.cpp Demo.cpp DemoWidget.cpp Scene1_Willy.cpp Willy.cpp Scene.cpp willy_bitmap.c \
	ImageDisplayDemoEntry.cpp SpectrumDemoEntry.cpp SpectrumSnaDemoEntry.cpp SceneDemoEntry.cpp
        
HEADERS=ImageWidget.h RemoteScreen.h Demo.h DemoWidget.h Willy.h Scene1_Willy.h Scene.h SceneDemoEntry.h DemoEntry.h

LIBS+=-L.. -L../QtSpecem -L../QtZeroConf -lQtSpecem -lQtZeroConf

QMAKE_RESOURCE_FLAGS += -no-compress

RESOURCES=zxload_demo.qrc
