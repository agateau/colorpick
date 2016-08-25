#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class ColorEditor;

class QLabel;

class Window : public QMainWindow
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = 0);

private:
    void onColorChanged();
    void updateRatioLabel();
    void updateDemoLabel();

    ColorEditor *mBgEditor;
    ColorEditor *mFgEditor;

    QLabel *mRatioLabel;
    QLabel *mDemoLabel;
};

#endif // WINDOW_H
