#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QDialog>

class QTimer;

class ColorPicker : public QDialog
{
    Q_OBJECT
public:
    ColorPicker();
    ~ColorPicker();

Q_SIGNALS:
    void colorChanged(const QColor &);

protected:
    void showEvent(QShowEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void updatePosition();
    void emitColorChanged();

    QTimer *mTimer;
    QPixmap mPixmap;
};

#endif // COLORPICKER_H
