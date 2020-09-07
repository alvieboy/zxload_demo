#include "DemoWidget.h"
#include "ImageWidget.h"
#include "SpectrumWidget.h"
#include "DemoEntry.h"

#define BORDER_HORIZONTAL 32
#define BORDER_VERTICAL 32


extern "C" unsigned char *mem;
extern "C" void init_pallete();
extern "C" void init_pallete_reversed();
extern "C" void init_emul();





DemoWidget::DemoWidget(): QStackedWidget()
{
    imageWidget = new ImageWidget();
    spectrumWidget = new SpectrumWidget();

    addWidget(imageWidget);
    addWidget(spectrumWidget);

    spectrumWidget->setMinimumSize(((256+(BORDER_HORIZONTAL*2))/2),
                                   (192+(BORDER_VERTICAL*2))/2);
    spectrumWidget->setTransformationMode(Qt::SmoothTransformation);
    spectrumWidget->setAspectRatio(Qt::KeepAspectRatio);

    QString filename;

    filename = "rom/spectrum.rom";

    printf("Using rom %s\n", filename.toLatin1().constData());
    QFile file(filename);

    printf("Init pallete\n");
#ifdef __arm__
    init_pallete_reversed();
#else
    init_pallete();
#endif
    printf("Init emul\n");
    init_emul();

    QByteArray data;
    const char *p;
    int i;

    if(file.open(QIODevice::ReadOnly)){
        data=file.readAll();
        file.close();
        p=data;
        for (i=0; i < 16384 ; i++)
            *(mem+i) = *(p++);
    } else {
        printf("Cannot open ROM file\n");
        return;
    }

    spectrumWidget->show();
    spectrumWidget->loadSNA("demo.sna");

    QObject::connect(&timer, &QTimer::timeout, this, &DemoWidget::timerExpired);
    timer.setSingleShot(true);
}



void DemoWidget::stop()
{
    if (demoindex>=0) {
        struct de entry = entries[demoindex];
        entry.entry->deactivate();
    }
    spectrumWidget->stopEmul();
    timer.stop();
    demoindex = -1;
}

void DemoWidget::start()
{
    demoindex=0;
    if (entries.size()>0)
        startDemo();
}

void DemoWidget::startDemo()
{
    struct de entry = entries[demoindex];
    timer.start(entry.timeout);
    entry.entry->activate();
}

void DemoWidget::timerExpired()
{
    qDebug()<<"Expired";

    struct de entry = entries[demoindex];
    entry.entry->deactivate();

    demoindex++;

    if (demoindex>=entries.size()) {
        demoindex=0;
    }
    startDemo();
}

void DemoWidget::addDemoEntry(DemoEntry*entry, unsigned timeout)
{
    struct de d;
    d.entry = entry;
    d.timeout = timeout;
    entries.push_back(d);
}

void DemoWidget::selectImage()
{
    spectrumWidget->stopEmul();
    setCurrentIndex(0);
}

void DemoWidget::selectSpectrum()
{
    setCurrentIndex(1);
}


