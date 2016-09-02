#include "window.h"

#include "coloreditor.h"
#include "contrastpreview.h"

#include <QVBoxLayout>

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    setWindowIcon(QIcon::fromTheme("fill-color"));

    QWidget *centralWidget = new QWidget;

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setSpacing(0);

    mBgEditor = new ColorEditor;

    mFgEditor = new ColorEditor;
    mFgEditor->setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, pal.color(QPalette::Window).dark(110));
    mFgEditor->setPalette(pal);

    ContrastPreview *preview = new ContrastPreview;

    connect(mBgEditor, &ColorEditor::colorChanged, preview, &ContrastPreview::setBackgroundColor);
    connect(mFgEditor, &ColorEditor::colorChanged, preview, &ContrastPreview::setForegroundColor);

    layout->setContentsMargins(QMargins());
    layout->addWidget(mBgEditor);
    layout->addWidget(mFgEditor);
    layout->addWidget(preview);

    setCentralWidget(centralWidget);
    mBgEditor->setColor(Qt::white);
    mFgEditor->setColor(Qt::black);
}
