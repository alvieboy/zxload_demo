#include "ImageDisplayDemoEntry.h"
#include "ImageWidget.h"

void ImageDisplayDemoEntry::activate(ImageWidget*w)
{
    w->load(file);
}

