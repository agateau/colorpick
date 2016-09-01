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
#include <QPainter>
#include <QPushButton>
#include <QToolButton>
#include <QWidgetAction>

/**
 * A QToolButton which has no border and immediatly shows a menu (there is no way to alter the popup mode of the button
 * created when adding an action directly to QLineEdit)
 */
class MenuLineEditButton : public QToolButton
{
public:
    MenuLineEditButton(QMenu* menu)
    {
        setMenu(menu);
        setPopupMode(QToolButton::InstantPopup);
        setCursor(Qt::ArrowCursor);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        QIcon::Mode state = isDown() ? QIcon::Selected : QIcon::Normal;
        QPixmap pix = icon().pixmap(size(), state, QIcon::Off);
        QRect pixRect = QRect(QPoint(0, 0), pix.size() / pix.devicePixelRatio());
        pixRect.moveCenter(rect().center());
        painter.drawPixmap(pixRect, pix);
    }
};

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

    setupCopyButton();

    QBoxLayout *componentEditorLayout = new QVBoxLayout;
    mRgbEditor = new ComponentEditor(RgbColorSpace::instance());
    connect(mRgbEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);
    mHsvEditor = new ComponentEditor(HsvColorSpace::instance());
    connect(mHsvEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(mColorButton, 0, 0);
    layout->addWidget(mLineEdit, 0, 1);
    layout->addWidget(pickerButton, 0, 2);

    componentEditorLayout->setContentsMargins(QMargins());
    componentEditorLayout->addWidget(mRgbEditor);
    componentEditorLayout->addWidget(mHsvEditor);
    layout->addLayout(componentEditorLayout, 1, 0, 1, 3);
}

void ColorEditor::setupCopyButton()
{
    mCopyMenu = new QMenu(this);
    connect(mCopyMenu, &QMenu::aboutToShow, this, &ColorEditor::fillCopyMenu);

    MenuLineEditButton *copyButton = new MenuLineEditButton(mCopyMenu);
    copyButton->setIcon(QIcon::fromTheme("edit-copy"));

    QWidgetAction *copyAction = new QWidgetAction(this);
    copyAction->setDefaultWidget(copyButton);
    mLineEdit->addAction(copyAction, QLineEdit::TrailingPosition);
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
