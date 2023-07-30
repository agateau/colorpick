#include "coloreditor.h"

#include "colorpicker.h"
#include "hsvcolorspace.h"
#include "rgbcolorspace.h"
#include "componenteditor.h"

#include <KColorButton>

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMargins>
#include <QMenu>
#include <QMimeData>
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

ColorEditor::ColorEditor(const QIcon &icon, QWidget *parent) : QWidget(parent)
{
    setAcceptDrops(true);

    QLabel *iconLabel = new QLabel;
    iconLabel->setPixmap(icon.pixmap(40, 40));

    mColorButton = new KColorButton();
    mColorButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
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
    pickerButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    pickerButton->setIcon(QIcon::fromTheme("color-picker"));
    connect(pickerButton, &QToolButton::clicked, this, &ColorEditor::startPicking);

    setupCopyButton();

    mRgbEditor = new ComponentEditor(RgbColorSpace::instance());
    connect(mRgbEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);
    mHsvEditor = new ComponentEditor(HsvColorSpace::instance());
    connect(mHsvEditor, &ComponentEditor::colorChanged, this, &ColorEditor::setColor);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(iconLabel, 0, 0, 2, 1, Qt::AlignTop);
    layout->addWidget(mColorButton, 0, 1);
    layout->addWidget(mLineEdit, 0, 2);
    layout->addWidget(pickerButton, 0, 3);

    QBoxLayout *componentEditorLayout = new QVBoxLayout;
    componentEditorLayout->setContentsMargins(QMargins());
    componentEditorLayout->addWidget(mRgbEditor);
    componentEditorLayout->addWidget(mHsvEditor);
    layout->addLayout(componentEditorLayout, 1, 1, 1, 3);
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
        emit colorChanged(mColor);
    }
}

void ColorEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasColor()) {
        event->acceptProposedAction();
    }
}

void ColorEditor::dropEvent(QDropEvent *event)
{
    QColor color = qvariant_cast<QColor>(event->mimeData()->colorData());
    setColor(color);
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
#if QT_VERSION < 0x060000
    typedef qreal float_t; // double
#else
    typedef float float_t;
#endif
    mCopyMenu->clear();
    int r, g, b;
    float_t rf, gf, bf;
    mColor.getRgb(&r, &g, &b);
    mColor.getRgbF(&rf, &gf, &bf);

    auto constexpr ftoa = [](float value) { return QString::number(value, 'g', 3); };
    auto constexpr htoa = [](int value) { return QString::number(value, 16).rightJustified(2, '0'); };
    auto constexpr itoa = [](int value) { return QString::number(value); };

    auto addColorAction = [this](const QString &text, const QString &value) {
        QString fullText = QString("%1: %2").arg(text, value);
        QAction *action = mCopyMenu->addAction(fullText);
        connect(action, &QAction::triggered, this, [value]() {
            QApplication::clipboard()->setText(value);
        });
    };

    addColorAction(tr("Inkscape"), htoa(r) + htoa(g) + htoa(b) + htoa(255));
    addColorAction(tr("Hexa with #"), "#" + htoa(r) + htoa(g) + htoa(b));
    addColorAction(tr("Quoted hexa with #"), "\"#" + htoa(r) + htoa(g) + htoa(b) + "\"");
    addColorAction(tr("Float values"), QString("%1, %2, %3").arg(ftoa(rf), ftoa(gf), ftoa(bf)));
    addColorAction(tr("Int values"), QString("%1, %2, %3").arg(itoa(r), itoa(g), itoa(b)));
    addColorAction(tr("CSS RGB Value"), QString("rgb(%1, %2, %3)").arg(itoa(r), itoa(g), itoa(b)));
}
