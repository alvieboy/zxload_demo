#include "SpectrumWidget.h"
#include "QtSpecem.h"
#include <sys/socket.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <unistd.h>
#include "interfacez/interfacez.h"
#include <QPlainTextEdit>


extern "C" unsigned char *mem;
extern "C" void init_pallete();
extern "C" void init_emul();


static int setupgui()
{
    SpectrumWidget *spectrumWidget = new SpectrumWidget();

    QMainWindow *mainw = new EmulatorWindow();

    //QWidget *mainwidget = new QWidget();

    mainw->setCentralWidget(spectrumWidget);
    mainw->setFixedSize(160,128);

#define BORDER_HORIZONTAL 32
#define BORDER_VERTICAL 32

    spectrumWidget->setMinimumSize(((256+(BORDER_HORIZONTAL*2))/2),
                (192+(BORDER_VERTICAL*2))/2);


    spectrumWidget->setTransformationMode(Qt::SmoothTransformation);
    spectrumWidget->setAspectRatio(Qt::KeepAspectRatio);
    mainw->show();

    QString filename;

    filename = "rom/spectrum.rom";

    printf("Using rom %s\n", filename.toLatin1().constData());
    QFile file(filename);

    printf("Init pallete\n");
    init_pallete_reversed();
    
    printf("Init emul\n");
    init_emul();

    QByteArray data;
    const char *p;
    int i;

    if(file.open(QIODevice::ReadOnly)){
        data=file.readAll();
        file.close();
        p=data;
        for (i=0; i < 16384 ; i++)
            *(mem+i) = *(p++);
    } else {
        printf("Cannot open ROM file\n");
        return -1;
    }

    spectrumWidget->show();
    spectrumWidget->loadSNA("demo.sna");
    return 0;
}


int main(int argc, char**argv)
{
    int sockets[2];
    QApplication app(argc, argv);
    
    setupgui();

    return app.exec();
};
