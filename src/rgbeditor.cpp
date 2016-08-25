#include "rgbeditor.h"

#include <KSelector>

#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>

RgbEditor::RgbEditor(QWidget *parent) : QWidget(parent)
{
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(QMargins());

    QString labelTexts[3] = {
        tr("R"),
        tr("G"),
        tr("B")
    };
    for (int row = 0; row < 3; ++row) {
        QLabel *label = new QLabel(labelTexts[row]);

        KGradientSelector *selector = new KGradientSelector();
        selector->setRange(0, 255);
        selector->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        selector->setArrowDirection(Qt::UpArrow);

        QSpinBox *spinBox = new QSpinBox();
        spinBox->setRange(0, 255);

        layout->addWidget(label, row, 0);
        layout->addWidget(selector, row, 1);
        layout->addWidget(spinBox, row, 2);

        connect(selector, &KGradientSelector::valueChanged, this, &RgbEditor::updateFromSelectors);
        connect(selector, &KGradientSelector::valueChanged, spinBox, &QSpinBox::setValue);
        connect(spinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), selector, &KGradientSelector::setValue);

        mComponentSelectors[row] = selector;
        mComponentSpinBoxes[row] = spinBox;
    }

    updateSelectorGradients();
}

QColor RgbEditor::color() const
{
    return QColor(
        mComponentSelectors[0]->value(),
        mComponentSelectors[1]->value(),
        mComponentSelectors[2]->value()
    );
}

void RgbEditor::setColor(const QColor &newColor)
{
    if (color() != newColor) {
        mComponentSelectors[0]->setValue(newColor.red());
        mComponentSelectors[1]->setValue(newColor.green());
        mComponentSelectors[2]->setValue(newColor.blue());
        updateSelectorGradients();
        colorChanged(newColor);
    }
}

void RgbEditor::updateSelectorGradients()
{
    QRgb baseRgb = color().rgb();
    for (int row = 0; row < 3; ++row) {
        int selectorMask = 0xFF << ((2 - row) * 8);
        QColor minColor = QColor(baseRgb & (0xFFFFFF ^ selectorMask));
        QColor maxColor = QColor(baseRgb | selectorMask);
        mComponentSelectors[row]->setColors(minColor, maxColor);
    }
}

void RgbEditor::updateFromSelectors()
{
    updateSelectorGradients();
    colorChanged(color());
}
