#include "contrastpreview.h"

#include <KColorUtils>

#include <QLabel>
#include <QLocale>
#include <QVBoxLayout>

// As per http://www.w3.org/TR/WCAG20/#visual-audio-contrast
static constexpr qreal ACCEPTABLE_CONTRAST_RATIO = 3;
static constexpr qreal GOOD_CONTRAST_RATIO = 4.5;

ContrastPreview::ContrastPreview(QWidget *parent) : QWidget(parent)
{
    mRatioLabel = new QLabel;

    mDemoLabel = new QLabel(tr("Contrast Example Text"));
    mDemoLabel->setAutoFillBackground(true);
    mDemoLabel->setFrameStyle(QFrame::Box);
    mDemoLabel->setMargin(6);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(mRatioLabel);
    layout->addWidget(mDemoLabel);
}

void ContrastPreview::setBackgroundColor(const QColor &color)
{
    if (mBackgroundColor != color) {
        mBackgroundColor = color;
        updatePreview();
    }
}

void ContrastPreview::setForegroundColor(const QColor &color)
{
    if (mForegroundColor != color) {
        mForegroundColor = color;
        updatePreview();
    }
}

void ContrastPreview::updatePreview()
{
    updateRatioLabel();
    updateDemoLabel();
}

void ContrastPreview::updateRatioLabel()
{
    qreal ratio = KColorUtils::contrastRatio(mBackgroundColor, mForegroundColor);
    QString level;
    if (ratio < ACCEPTABLE_CONTRAST_RATIO) {
        level = tr("Bad");
    } else if (ratio < GOOD_CONTRAST_RATIO) {
        level = tr("Acceptable");
    } else {
        level = tr("Good");
    }
    QString text = tr("Contrast Ratio: %1:1 (%2)").arg(QLocale::system().toString(ratio, 'g', 2)).arg(level);
    mRatioLabel->setText(text);
}

void ContrastPreview::updateDemoLabel()
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, mBackgroundColor);
    pal.setColor(QPalette::WindowText, mForegroundColor);
    mDemoLabel->setPalette(pal);
}
