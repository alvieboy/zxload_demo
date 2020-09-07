#include "DemoEntry.h"
#include "SpectrumWidget.h"
#include "DemoWidget.h"

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
