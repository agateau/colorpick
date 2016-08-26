#include "window.h"

#include "coloreditor.h"

#include <KColorUtils>

#include <QLabel>
#include <QLocale>
#include <QVBoxLayout>

// As per http://www.w3.org/TR/WCAG20/#visual-audio-contrast
static constexpr qreal ACCEPTABLE_CONTRAST_RATIO = 3;
static constexpr qreal GOOD_CONTRAST_RATIO = 4.5;

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon::fromTheme("fill-color"));

    QWidget *centralWidget = new QWidget;

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    mBgEditor = new ColorEditor;
    mFgEditor = new ColorEditor;
    mRatioLabel = new QLabel;
    mDemoLabel = new QLabel(tr("Hello World!"));
    mDemoLabel->setAutoFillBackground(true);
    mDemoLabel->setFrameStyle(QFrame::Box);
    mDemoLabel->setMargin(6);

    connect(mBgEditor, &ColorEditor::colorChanged, this, &Window::onColorChanged);
    connect(mFgEditor, &ColorEditor::colorChanged, this, &Window::onColorChanged);

    layout->addWidget(mBgEditor);
    layout->addWidget(mFgEditor);
    layout->addWidget(mRatioLabel);
    layout->addWidget(mDemoLabel);

    setCentralWidget(centralWidget);
    mBgEditor->setColor(Qt::white);
    mFgEditor->setColor(Qt::black);
}

void Window::onColorChanged()
{
    updateRatioLabel();
    updateDemoLabel();
}

void Window::updateRatioLabel()
{
    qreal ratio = KColorUtils::contrastRatio(mBgEditor->color(), mFgEditor->color());
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

void Window::updateDemoLabel()
{
    QPalette pal = QPalette();
    pal.setColor(QPalette::Window, mBgEditor->color());
    pal.setColor(QPalette::WindowText, mFgEditor->color());
    mDemoLabel->setPalette(pal);
}
