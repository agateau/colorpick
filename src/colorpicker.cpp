#include "colorpicker.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QDesktopWidget>
#include <QImage>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QScreen>
#include <QShowEvent>
#include <QTimer>

static constexpr int GRAB_RADIUS = 16;
static constexpr int GRAB_SIZE = GRAB_RADIUS * 2 + 1;
static constexpr int MAGNIFY = 4;

static qreal scaleFactor()
{
    QScreen *screen = QGuiApplication::screenAt(QCursor::pos());
    if (screen) {
        return screen->devicePixelRatio();
    }
    return 1.0;
}

ColorPicker::ColorPicker()
{
    setWindowFlags(Qt::Window | Qt::X11BypassWindowManagerHint);
    setAttribute(Qt::WA_DeleteOnClose);
    setFixedSize(GRAB_SIZE * MAGNIFY, GRAB_SIZE * MAGNIFY);

    mScaleFactor = scaleFactor();

    mTimer = new QTimer(this);
    mTimer->setInterval(10);
    connect(mTimer, &QTimer::timeout, this, &ColorPicker::updatePosition);
}

ColorPicker::~ColorPicker()
{
    releaseMouse();
    releaseKeyboard();
}

void ColorPicker::showEvent(QShowEvent *)
{
    updatePosition();
    mTimer->start();
    QTimer::singleShot(100, this, [this]() {
        grabKeyboard();
        grabMouse(Qt::CrossCursor);
    });
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emitColorChanged();
        close();
    }
}

void ColorPicker::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    int dx = 0;
    int dy = 0;
    if (key == Qt::Key_Left) {
        dx = -1;
    } else if (key == Qt::Key_Right) {
        dx = 1;
    } else if (key == Qt::Key_Up) {
        dy = -1;
    } else if (key == Qt::Key_Down) {
        dy = 1;
    } else if (key == Qt::Key_Return) {
        emitColorChanged();
        close();
        return;
    } else if (key == Qt::Key_Escape) {
        close();
        return;
    } else {
        return;
    }

    QPoint pos = QCursor::pos();
    QCursor::setPos(pos.x() + dx, pos.y() + dy);
}

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Draw content
    QPixmap pix = mPixmap.scaled(GRAB_SIZE * mScaleFactor * MAGNIFY,
                                 GRAB_SIZE * mScaleFactor * MAGNIFY);
    painter.drawPixmap(0, 0, pix);

    // Draw outer border
    painter.setPen(Qt::darkGray);
    QRect rct = rect().adjusted(0, 0, -1, -1);
    painter.drawRect(rct);

    // Draw inner border
    painter.setPen(Qt::white);
    rct = rct.adjusted(1, 1, -1, -1);
    painter.drawRect(rct);

    // Draw centered cursor
    painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
    painter.drawRect(GRAB_RADIUS * MAGNIFY - 1, GRAB_RADIUS * MAGNIFY - 1, MAGNIFY + 1, MAGNIFY + 1);
}

void ColorPicker::emitColorChanged()
{
    QImage image = mPixmap.toImage();
    QColor color = image.pixelColor(GRAB_RADIUS * mScaleFactor, GRAB_RADIUS * mScaleFactor);
    emit colorChanged(color);
}

void ColorPicker::updatePosition()
{
    QPoint pos = QCursor::pos();
    QScreen *screen = QGuiApplication::screenAt(pos);
    if (!screen) {
        qWarning() << "Could not find a screen containing" << pos;
        return;
    }
    QRect desktopRect = screen->geometry();

    QPoint newPos;
    if (pos.x() + GRAB_SIZE + width() < desktopRect.width()) {
        newPos.setX(pos.x() + GRAB_SIZE);
    } else {
        newPos.setX(pos.x() - GRAB_SIZE - width());
    }
    if (pos.y() + GRAB_SIZE + height() < desktopRect.height()) {
        newPos.setY(pos.y() + GRAB_SIZE);
    } else {
        newPos.setY(pos.y() - GRAB_SIZE - height());
    }

    move(newPos);

    WId wid = QApplication::desktop()->winId();

    mPixmap = screen->grabWindow(wid, pos.x() - GRAB_SIZE / 2, pos.y() - GRAB_SIZE / 2, GRAB_SIZE, GRAB_SIZE);
    update();
}
