#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
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

    QPushButton *mColorButton;
    QLineEdit *mLineEdit;

    QLabel *mLuminanceLabel;

    QColor mColor;
};

#endif // COLOREDITOR_H
