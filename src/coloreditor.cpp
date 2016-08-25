#include "coloreditor.h"

#include "rgbeditor.h"

#include <KColorButton>
#include <KColorUtils>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>

ColorEditor::ColorEditor(QWidget *parent) : QWidget(parent)
{
    mColorButton = new KColorButton();
    connect(mColorButton, &KColorButton::changed, this, &ColorEditor::setColor);

    mLineEdit = new QLineEdit();
    connect(mLineEdit, &QLineEdit::textEdited, this, [this](const QString &text) {
        if (QColor::isValidColor(text)) {
            setColor(QColor(text));
        }
    });

    QToolButton *darkerButton = createValueButton(-10);
    QToolButton *lighterButton = createValueButton(10);

    QToolButton *pickerButton = new QToolButton();
    pickerButton->setIcon(QIcon::fromTheme("color-picker"));
    pickerButton->setFocusPolicy(Qt::TabFocus);
    connect(pickerButton, &QToolButton::clicked, this, &ColorEditor::startPicking);

    QToolButton *copyButton = new QToolButton();
    copyButton->setIcon(QIcon::fromTheme("edit-copy"));
    copyButton->setFocusPolicy(Qt::TabFocus);

    mCopyMenu = new QMenu(this);
    copyButton->setMenu(mCopyMenu);
    copyButton->setPopupMode(QToolButton::InstantPopup);
    connect(mCopyMenu, &QMenu::aboutToShow, this, &ColorEditor::fillCopyMenu);

    mRgbEditor = new RgbEditor();
    connect(mRgbEditor, &RgbEditor::colorChanged, this, &ColorEditor::setColor);

    mLuminanceLabel = new QLabel();

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(QMargins());
    layout->addWidget(mColorButton, 0, 0);
    layout->addWidget(mLineEdit, 0, 1);
    layout->addWidget(darkerButton, 0, 2);
    layout->addWidget(lighterButton, 0, 3);
    layout->addWidget(pickerButton, 0, 4);
    layout->addWidget(copyButton, 0, 5);

    layout->addWidget(mRgbEditor, 1, 0, 1, 6);

    layout->addWidget(mLuminanceLabel, 2, 0, 1, 6);
}

QColor ColorEditor::color() const
{
    return mColor;
}

void ColorEditor::setColor(const QColor &color)
{
    if (mColor != color) {
        mColor = color;
        updateFromColor();
        colorChanged(mColor);
    }
}

void ColorEditor::updateFromColor()
{
    mColorButton->setColor(mColor);

    if (!mLineEdit->hasFocus()) {
        mLineEdit->setText(mColor.name());
    }

    mRgbEditor->setColor(mColor);

    qreal luminance = KColorUtils::luma(mColor);
    QString lumaText = tr("Luminance: %1").arg(QLocale::system().toString(luminance, 'f', 3));
    mLuminanceLabel->setText(lumaText);
}

void ColorEditor::adjustValue(int delta)
{
    int h, s, v, a;
    mColor.getHsv(&h, &s, &v, &a);
    v = qBound(0, v + delta, 255);
    setColor(QColor::fromHsv(h, s, v, a));
}

void ColorEditor::startPicking()
{

}

void ColorEditor::fillCopyMenu()
{
    mCopyMenu->clear();
    int r, g, b;
    qreal rf, gf, bf;
    mColor.getRgb(&r, &g, &b);
    mColor.getRgbF(&rf, &gf, &bf);

    auto myfloat = [](qreal value) {
        return QString::number(value, 'f', 3);
    };

    auto hex = [](int value) {
        return QString::number(value, 16).rightJustified(2, '0');
    };

    auto addColorAction = [this](const QString &text, const QString &value) {
        QString fullText = ColorEditor::tr("%1: %2").arg(text).arg(value);
        QAction *action = mCopyMenu->addAction(fullText);
        connect(action, &QAction::triggered, this, [value]() {
            QApplication::clipboard()->setText(value);
        });
    };

    addColorAction(tr("Inkscape"), hex(r) + hex(g) + hex(b) + hex(255));
    addColorAction(tr("Hexa with #"), "#" + hex(r) + hex(g) + hex(b));
    addColorAction(tr("Quoted hexa with #"), "\"#" + hex(r) + hex(g) + hex(b) + "\"");
    addColorAction(tr("Float values"), QString("%1, %2, %3").arg(myfloat(r)).arg(myfloat(g)).arg(myfloat(b)));
}

QToolButton *ColorEditor::createValueButton(int delta)
{
    QToolButton *button = new QToolButton();
    button->setFocusPolicy(Qt::TabFocus);
    button->setArrowType(delta < 0 ? Qt::DownArrow : Qt::UpArrow);
    connect(button, &QToolButton::clicked, this, [this, delta]() {
        adjustValue(delta);
    });
    return button;
}
