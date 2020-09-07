#ifndef __DEMOWIDGET_H__
#define __DEMOWIDGET_H__

#include <QStackedWidget>
#include <QTimer>
#include <QVector>
#include "Scene.h"

class ImageWidget;
class SpectrumWidget;

class DemoEntry;

class DemoWidget: public QStackedWidget
{
public:
    DemoWidget();
    void stop();
    void start();
    ImageWidget *image() { return imageWidget; }
    SpectrumWidget *spectrum() { return spectrumWidget; }

    void selectImage();
    void selectSpectrum();
    void addDemoEntry(DemoEntry*, unsigned timeout);
public slots:
    void timerExpired();
protected:
    void startDemo();
    ImageWidget *imageWidget;
    SpectrumWidget *spectrumWidget;
    QTimer timer;
    struct de {
        DemoEntry *entry;
        unsigned timeout;
    };
    QVector<struct de> entries;
    int demoindex;
};

#endif