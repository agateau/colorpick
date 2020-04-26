#ifndef IMAGEGRADIENTSELECTOR_H
#define IMAGEGRADIENTSELECTOR_H

#include <KSelector>

/**
 * A KSelector which displays a stretched image instead of a gradient. Useful to create gradients which are not
 * RGB-based.
 */
class ImageGradientSelector : public KSelector
{
public:
    ImageGradientSelector(QWidget *parent = nullptr);

    void setImage(const QImage &image);

protected:
    void drawContents(QPainter *painter) override;

private:
    QImage mImage;
};

#endif // IMAGEGRADIENTSELECTOR_H
