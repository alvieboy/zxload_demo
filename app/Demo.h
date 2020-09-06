#include <QObject>

class QStackedWidget;
class RemoteScreen;
class QMainWindow;
class DemoWidget;

class Demo: public QObject
{
    Q_OBJECT
public:
    Demo();
    void loadDemos();
public slots:
    void connectionDetected();
    void connectionLost();

private:
    QStackedWidget *stack;
    RemoteScreen *rs;
    QMainWindow *mainw;
    DemoWidget *demow;
};
