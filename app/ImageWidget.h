#include <QWidget>
#include <QImage>
#include "ScreenDrawer.h"
#include <QPainter>

class ImageWidget: public QWidget, public ScreenDrawer
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent=NULL);
    void paintEvent(QPaintEvent *);
    void load(const char *file);
    void allocEmpty();
    void clear();

    virtual void drawPixel(int x, int y, uint32_t color);
    virtual void drawImage(int x, int y, QImage *);
    virtual void drawHLine(int x, int y, int width, uint32_t color);
    virtual void drawVLine(int x, int y, int width, uint32_t color);
    virtual void startFrame();
    virtual void finishFrame();
    virtual QVideoWidget* getVideoWidget();
    virtual void setVideoMode(bool);


protected:
    QImage image;
};
