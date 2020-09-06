#include <QWidget>
#include <QImage>

class ImageWidget: public QWidget
{
    Q_OBJECT
public:
    ImageWidget(QWidget *parent=NULL);
    void paintEvent(QPaintEvent *);
    void load(const char *file);
protected:
    QImage image;
};
