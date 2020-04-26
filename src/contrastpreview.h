#ifndef CONTRASTPREVIEW_H
#define CONTRASTPREVIEW_H

#include <QWidget>

class QLabel;

/**
 * Shows a preview of what a text would look like with the given foreground and background colors
 */
class ContrastPreview : public QWidget
{
    Q_OBJECT
public:
    explicit ContrastPreview(QWidget *parent = 0);

    void setBackgroundColor(const QColor &color);
    void setForegroundColor(const QColor &color);

private:
    void updatePreview();
    void updateRatioLabel();
    void updateDemoLabel();

    QColor mBackgroundColor;
    QColor mForegroundColor;

    QLabel *mRatioLabel;
    QLabel *mDemoLabel;
};

#endif // CONTRASTPREVIEW_H
