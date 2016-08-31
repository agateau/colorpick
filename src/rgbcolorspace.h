#ifndef RGBCOLORSPACE_H
#define RGBCOLORSPACE_H

#include "colorspace.h"

class RgbColorSpace : public ColorSpace
{
public:
    static RgbColorSpace *instance();

    QString name(int idx) const override;
    int value(const QColor &color, int idx) const override;
    int maximum(int idx) const override;

    QVector<int> values(const QColor &color) const override;
    QColor fromValues(const QVector<int> &values) const override;

private:
    RgbColorSpace();
};

#endif // RGBCOLORSPACE_H
