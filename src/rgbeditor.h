#ifndef RGBEDITOR_H
#define RGBEDITOR_H

#include <QWidget>

class KGradientSelector;

class QSpinBox;

class RgbEditor : public QWidget
{
    Q_OBJECT
public:
    explicit RgbEditor(QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

Q_SIGNALS:
    void colorChanged(const QColor &color);

private:
    void updateSelectorGradients();
    void updateFromSelectors();

    KGradientSelector *mComponentSelectors[3];
    QSpinBox *mComponentSpinBoxes[3];
};

#endif // RGBEDITOR_H
