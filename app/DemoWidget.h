#include <QStackedWidget>
#include <QTimer>
#include <QVector>

class ImageWidget;
class SpectrumWidget;

class DemoWidget;

class DemoEntry
{
public:
    DemoEntry(DemoWidget*d): w(d)  {}
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    DemoWidget *w;
};

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



class ImageDemoEntry: public DemoEntry
{
public:
    ImageDemoEntry(DemoWidget *d): DemoEntry(d)
    {
    }

    virtual void activate()
    {
        w->selectImage();
        activate(w->image());
    }
    virtual void activate(ImageWidget *) = 0;
};

class SpectrumDemoEntry: public DemoEntry
{
public:
    SpectrumDemoEntry(DemoWidget *d): DemoEntry(d)
    {
    }

    virtual void activate()
    {
        w->selectSpectrum();
        activate(w->spectrum());
    }
    virtual void deactivate();
    virtual void activate(SpectrumWidget *) = 0;
};

class ImageDisplayDemoEntry: public ImageDemoEntry
{
public:
    ImageDisplayDemoEntry(DemoWidget *d, const char *filename): ImageDemoEntry(d), file(filename) {}
    virtual void activate(ImageWidget*w);
    virtual void deactivate() {}
private:
    const char *file;
};

class SpectrumSnaDemoEntry: public SpectrumDemoEntry
{
public:
    SpectrumSnaDemoEntry(DemoWidget *d, const char *filename): SpectrumDemoEntry(d), file(filename)
    {
    }

    virtual void activate(SpectrumWidget *);
private:
    const char *file;
};
