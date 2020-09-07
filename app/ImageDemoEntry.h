#include "DemoWidget.h"
#include "DemoEntry.h"

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
