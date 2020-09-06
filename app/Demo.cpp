#include "Demo.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "RemoteScreen.h"
#include "DemoWidget.h"
#include "QtSpecem.h"

Demo::Demo()
{
    QMainWindow *mainw = new EmulatorWindow();

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
    demow->addDemoEntry( new ImageDisplayDemoEntry(demow, "spectrum1.png"), 5000 );
    demow->addDemoEntry( new SpectrumSnaDemoEntry(demow, "manic.sna"), 5000 );
}

