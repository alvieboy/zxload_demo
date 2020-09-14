#include "Demo.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "RemoteScreen.h"
#include "DemoWidget.h"
#include "QtSpecem.h"
#include "Scene.h"
#include "ImageDisplayDemoEntry.h"
#include "SpectrumSnaDemoEntry.h"
#include "SceneDemoEntry.h"
#include "Scene1_Willy.h"

Demo::Demo()
{
    QMainWindow *mainw = new EmulatorWindow();

    mainw->setWindowState(Qt::WindowFullScreen);

    stack = new QStackedWidget();

    rs = new RemoteScreen();


    QObject::connect(rs, &RemoteScreen::connectionDetected, this, &Demo::connectionDetected);
    QObject::connect(rs, &RemoteScreen::connectionLost, this, &Demo::connectionLost);

    demow = new DemoWidget();

    stack->addWidget(demow);
    stack->addWidget(rs);

    rs->start();

    // TEST
    {
        QFile in("test.scr");
        in.open(QIODevice::ReadOnly);
        QByteArray data = in.readAll();
        rs->renderScreen((const scr_t*)data.constData(), false);
    }


    mainw->setCentralWidget(stack);
    mainw->setFixedSize(160,128);

    loadDemos();

    demow->start();

    mainw->show();
}


void Demo::connectionDetected()
{
    demow->stop();
    stack->setCurrentIndex(1);
}

void Demo::connectionLost()
{
    demow->start();
    stack->setCurrentIndex(0);
}



void Demo::loadDemos()
{
    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum.png"), 4000 );
    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "cantanhede1.png"), 4000 );

    Scene *willy = new Scene1_Willy();
    demow->addDemoEntry( new SceneDemoEntry(demow, willy), 40000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser1.png"), 6000 );
    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "manic.sna"), 30000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum4.png"), 6000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser2.png"), 6000 );

    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "backtoskool.sna"), 30000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum.png"), 4000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser3.png"), 6000 );

    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "thegreatescape.sna"), 20000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum4.png"), 4000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser4.png"), 6000 );

    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "chequeredflag.sna"), 20000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum.png"), 4000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser2.png"), 6000 );

    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "madmix.sna"), 20000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum4.png"), 4000 );

    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "teaser4.png"), 6000 );

    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "rocknroller.sna"), 20000 );
}


/*
              ['root','spectrum.png', 4],
             ['emul','loader.sna -load-immed intro.tap', 60, 4],
             ['img','teaser1.png', 4, 6],
             ['emul','manic.sna',30, 4],
             ['root','spectrum4.png', 4],
             ['img','teaser2.png', 6, 4],
             ['emul','backtoskool.sna',30, 4],
             ['root','spectrum.png', 4],
             ['img','teaser3.png', 6, 4],
             ['emul','thegreatescape.sna',20, 4],
             ['root','spectrum4.png', 4],
             ['img','teaser4.png', 6, 4],
             ['emul','chequeredflag.sna',20, 4],
             ['root','spectrum.png', 4],
             ['img','teaser2.png', 6, 4],
             ['emul','madmix.sna',20, 4],
             ['root','spectrum4.png', 4],
             ['img','teaser4.png', 6, 4],
             ['emul','rocknroller.sna',100, 4],
             ['root','spectrum.png', 4],
 */