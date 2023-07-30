#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class ColorEditor;

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private:
    ColorEditor *mBgEditor;
    ColorEditor *mFgEditor;
};

#endif // WINDOW_H
