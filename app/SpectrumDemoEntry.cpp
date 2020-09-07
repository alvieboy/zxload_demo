#include "SpectrumDemoEntry.h"

void SpectrumDemoEntry::deactivate()
{
    w->spectrum()->stopEmul();
}
