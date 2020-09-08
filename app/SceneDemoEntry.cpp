#include "SceneDemoEntry.h"
#include "Scene.h"
#include "DemoWidget.h"
#include "ImageWidget.h"
#include <QDebug>

#define FRAMEDELAY 60
//#define FRAMEDELAY 1


SceneDemoEntry::SceneDemoEntry(DemoWidget *d, Scene *s): DemoEntry(d), scene(s)
{
    QObject::connect(&timer, &QTimer::timeout, this, &SceneDemoEntry::tick);
    timer.setSingleShot(false);
}

//timer->start(FRAMEDELAY);

void SceneDemoEntry::activate()
{
    qDebug()<<"ACTIVATE scene";
    /*
    QObject::connect(scene, &Scene::sceneFinished,
                     this, &SceneDemoEntry::sceneFinished);
                     */
    w->image()->allocEmpty();
    w->image()->clear();

    scene->reset();
    scene->start();
    timer.stop();
    timer.start(FRAMEDELAY);
}

void SceneDemoEntry::deactivate()
{
    timer.stop();
}

void SceneDemoEntry::tick()
{
    ScreenDrawer *drawer = w->image();

    drawer->startFrame();

    scene->tick();

    scene->drawTo(drawer);

    drawer->finishFrame();
}
