#ifndef RGBEDITOR_H
#define RGBEDITOR_H

#include <QWidget>

class ColorSpace;
class ImageGradientSelector;

class QSpinBox;

class RgbEditor : public QWidget
{
    Q_OBJECT
public:
    explicit RgbEditor(ColorSpace *colorSpace, QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

Q_SIGNALS:
    void colorChanged(const QColor &color);

private:
    void updateSelectorGradients();
    void updateFromSelectors();

    QImage createGradientImage(int idx) const;

    ColorSpace *mColorSpace;
    ImageGradientSelector *mComponentSelectors[3];
    QSpinBox *mComponentSpinBoxes[3];
};

#endif // RGBEDITOR_H
