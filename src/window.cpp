#include "window.h"

#include "coloreditor.h"

#include <QVBoxLayout>

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget;

    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    mBgEditor = new ColorEditor;
    mFgEditor = new ColorEditor;

    layout->addWidget(mBgEditor);
    layout->addWidget(mFgEditor);

    setCentralWidget(centralWidget);
}

