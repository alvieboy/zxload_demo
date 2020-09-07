#include "ImageDemoEntry.h"

class ImageDisplayDemoEntry: public ImageDemoEntry
{
public:
    ImageDisplayDemoEntry(DemoWidget *d, const char *filename): ImageDemoEntry(d), file(filename) {}
    virtual void activate(ImageWidget*w);
    virtual void deactivate() {}
private:
    const char *file;
};
