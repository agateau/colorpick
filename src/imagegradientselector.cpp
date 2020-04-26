#include "imagegradientselector.h"

#include <QPainter>

ImageGradientSelector::ImageGradientSelector(QWidget *parent)
    : KSelector(parent)
{

}

void ImageGradientSelector::setImage(const QImage &image)
{
    mImage = image;
    update();
}

void ImageGradientSelector::drawContents(QPainter *painter)
{
    painter->drawImage(contentsRect(), mImage);
}
