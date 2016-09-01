#include "coloreditor.h"

#include "colorpicker.h"
#include "hsvcolorspace.h"
#include "rgbcolorspace.h"
#include "componenteditor.h"

#include <KColorButton>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMargins>
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
            mFromLineEdit = true;
            setColor(QColor(text));
            mFromLineEdit = false;
        }
    });

    QToolButton *pickerButton = new QToolButton();
    pickerButton->setIcon(QIcon::fromTheme("color-picker"));
    connect(pickerButton, &QToolButton::clicked, this, &ColorEditor::startPicking);

    QToolButton *copyButton = new QToolButton();
    copyButton->setIcon(QIcon::fromTheme("edit-copy"));

    mCopyMenu = new QMenu(this);
    copyButton->setMenu(mCopyMenu);
    copyButton->setPopupMode(QToolButton::InstantPopup);
    connect(mCopyMenu, &QMenu::aboutToShow, this, &ColorEditor::fillCopyMenu);

    QBoxLayout *componentEditorLayout = new QVBoxLayout;
    mRgbEditor = new ComponentEditor(RgbColorSpace::instance());
    connect(mRgbEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);
    mHsvEditor = new ComponentEditor(HsvColorSpace::instance());
    connect(mHsvEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mColorButton, 0, 0);
    layout->addWidget(mLineEdit, 0, 1);
    layout->addWidget(pickerButton, 0, 2);
    layout->addWidget(copyButton, 0, 3);

    componentEditorLayout->setContentsMargins(QMargins());
    componentEditorLayout->addWidget(mRgbEditor);
    componentEditorLayout->addWidget(mHsvEditor);
    layout->addLayout(componentEditorLayout, 1, 0, 1, 4);
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

    if (!mFromLineEdit) {
        mLineEdit->setText(mColor.name());
    }

    mRgbEditor->setColor(mColor);
    mHsvEditor->setColor(mColor);
}

void ColorEditor::startPicking()
{
    ColorPicker *picker = new ColorPicker;
    connect(picker, &ColorPicker::colorChanged, this, &ColorEditor::setColor);
    picker->exec();
}

void ColorEditor::fillCopyMenu()
{
    mCopyMenu->clear();
    int r, g, b;
    qreal rf, gf, bf;
    mColor.getRgb(&r, &g, &b);
    mColor.getRgbF(&rf, &gf, &bf);

    auto myfloat = [](qreal value) {
        return QString::number(value, 'g', 3);
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
    addColorAction(tr("Float values"), QString("%1, %2, %3").arg(myfloat(rf)).arg(myfloat(gf)).arg(myfloat(bf)));
}
