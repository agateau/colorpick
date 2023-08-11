#include "hsvcolorspace.h"

#include <QCoreApplication>

HsvColorSpace *HsvColorSpace::instance()
{
    static HsvColorSpace obj;
    return &obj;
}

HsvColorSpace::HsvColorSpace()
{
}

QString HsvColorSpace::name(int idx) const
{
    static QString names[3] = {
        QCoreApplication::translate("HsvColorSpace", "H"),
        QCoreApplication::translate("HsvColorSpace", "S"),
        QCoreApplication::translate("HsvColorSpace", "V"),
    };
    return names[idx];
}

int HsvColorSpace::value(const QColor &color, int idx) const
{
    if (idx == 0) {
        return color.hue();
    } else if (idx == 1) {
        return color.saturation();
    } else if (idx == 2) {
        return color.value();
    } else {
        return color.alpha();
    }
}

int HsvColorSpace::maximum(int idx) const
{
    return idx == 0 ? 359 : 255;
}

QVector<int> HsvColorSpace::values(const QColor &color) const
{
    return { color.hue(), color.saturation(), color.value() };
}

QColor HsvColorSpace::fromValues(const QVector<int> &values) const
{
    return QColor::fromHsv(values.at(0), values.at(1), values.at(2));
}
