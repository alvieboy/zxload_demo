#include "SpectrumSnaDemoEntry.h"

void SpectrumSnaDemoEntry::activate(SpectrumWidget*w)
{
    w->loadSNA(file);
    w->resumeEmul();
}
