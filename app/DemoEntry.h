#ifndef __DEMOENTRY_H__
#define __DEMOENTRY_H__

#include <QObject>

class DemoWidget;

class DemoEntry: public QObject
{
    Q_OBJECT
public:
    DemoEntry(DemoWidget*d): w(d)  {}
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    DemoWidget *w;
};

#endif