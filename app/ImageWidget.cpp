#include "ImageWidget.h"
#include <QPainter>
#include <QPaintEvent>
#include <QDebug>

ImageWidget::ImageWidget(QWidget *parent): QWidget(parent) {
    setMinimumSize( 160, 128 );
}

void ImageWidget::paintEvent(QPaintEvent *) 
{
    QPainter paint(this);
    paint.drawImage(0, 0, image.scaled(size(),
                                       Qt::KeepAspectRatio,
                                       Qt::FastTransformation));

}

void ImageWidget::load(const char *file)
{
    image.load(file);
    repaint();
}
