#ifndef COLOREDITOR_H
#define COLOREDITOR_H

#include <QWidget>

class ComponentEditor;

class KColorButton;

class QLabel;
class QLineEdit;
class QMenu;
class QToolButton;

class ColorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ColorEditor(const QIcon &icon, QWidget *parent = 0);

    QColor color() const;
    void setColor(const QColor &color);

Q_SIGNALS:
    void colorChanged(const QColor &color);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void updateFromColor();
    void startPicking();
    void fillCopyMenu();

    KColorButton *mColorButton;
    QLineEdit *mLineEdit;
    QMenu *mCopyMenu;

    ComponentEditor *mRgbEditor;
    ComponentEditor *mHsvEditor;

    QColor mColor;

    bool mFromLineEdit = false;
    void setupCopyButton();
};

#endif // COLOREDITOR_H
