#include "ImageWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent): QWidget(parent) {
    setMinimumSize( 160, 128 );
}

void ImageWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, image.scaled(size(),
                                         Qt::KeepAspectRatio,
                                         Qt::FastTransformation));

}

void ImageWidget::clear()
{
    image.fill(0x0);
}

void ImageWidget::load(const char *file)
{
    image.load(file);
    repaint();
}

void ImageWidget::allocEmpty()
{
    image = QImage(160, 128, QImage::Format_RGB32);
}

void ImageWidget::drawPixel(int x, int y, uint32_t color)
{
    if (x<160 && y<128 && x>=0 && y>=0) {
        image.setPixel(x,y,color);
    }
}

void ImageWidget::drawImage(int x, int y, QImage *i)
{
    QPainter p(&image);
    p.drawImage(x,y,*i);
}

void ImageWidget::drawHLine(int x, int y, int width, uint32_t color)
{
    while (width--) {
        drawPixel(x,y,color);
        x++;
    }
}

void ImageWidget::drawVLine(int x, int y, int height, uint32_t color)
{
    while (height--) {
        drawPixel(x,y,color);
        y++;
    }
}

void ImageWidget::startFrame()
{
    clear();
}
void ImageWidget::finishFrame()
{
    repaint();
}

QVideoWidget* ImageWidget::getVideoWidget()
{
    return NULL;
}

void ImageWidget::setVideoMode(bool)
{
}
