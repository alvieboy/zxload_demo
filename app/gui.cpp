#include "Demo.h"
#include <QApplication>

int main(int argc, char**argv)
{
    QApplication app(argc, argv);
    
    Demo *d = new Demo();

    int r = app.exec();

    delete(d);

    return r;
};
