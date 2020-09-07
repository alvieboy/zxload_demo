#include "DemoEntry.h"
#include <QTimer>

class Scene;

class SceneDemoEntry: public DemoEntry
{
public:
    SceneDemoEntry(DemoWidget *d, Scene *s);
    virtual void activate() override;
    virtual void deactivate() override;
    void tick();
private:
    Scene *scene;
    QTimer timer;
};
