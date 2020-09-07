#include "SpectrumDemoEntry.h"

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

