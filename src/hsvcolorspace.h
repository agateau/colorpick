#ifndef HSVCOLORSPACE_H
#define HSVCOLORSPACE_H

#include "colorspace.h"

class HsvColorSpace : public ColorSpace
{
public:
    static HsvColorSpace *instance();

    QString name(int idx) const override;
    int value(const QColor &color, int idx) const override;
    int maximum(int idx) const override;

    QVector<int> values(const QColor &color) const override;
    QColor fromValues(const QVector<int> &values) const override;

private:
    HsvColorSpace();
};

#endif // HSVCOLORSPACE_H
