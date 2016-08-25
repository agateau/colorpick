#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>

class RgbEditor;

class KColorButton;

class QLabel;
class QLineEdit;
class QMenu;
class QToolButton;

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

Q_SIGNALS:
    void colorChanged(const QColor &color);

private:
    QToolButton *createValueButton(int delta);
    void updateFromColor();
    void adjustValue(int delta);
    void startPicking();
    void fillCopyMenu();

    KColorButton *mColorButton;
    QLineEdit *mLineEdit;
    QMenu *mCopyMenu;

    RgbEditor *mRgbEditor;

    QLabel *mLuminanceLabel;

    QColor mColor;
};

#endif // COLOREDITOR_H
