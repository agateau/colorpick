from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

GRAB_RADIUS = 16
GRAB_SIZE = GRAB_RADIUS * 2 + 1
MAGNIFY = 4

class ColorPicker(QDialog):
    picked = pyqtSignal(QColor)

    def __init__(self, parent = None):
        QDialog.__init__(self, parent)
        self.setWindowFlags(Qt.Window | Qt.X11BypassWindowManagerHint)
        self.setFixedSize(GRAB_SIZE * MAGNIFY, GRAB_SIZE * MAGNIFY)
        self.timer = QTimer(self)
        self.timer.setInterval(10)
        self.timer.timeout.connect(self.updatePosition)
        self.pix = QPixmap()

    def showEvent(self, event):
        self.updatePosition()
        self.timer.start()
        QTimer.singleShot(100, self.grab)

    def grab(self):
        self.grabKeyboard()
        self.grabMouse(Qt.CrossCursor)

    def mousePressEvent(self, event):
        if event.button() == Qt.LeftButton:
            self.emitPicked()
        self.close()

    def close(self):
        self.releaseMouse()
        self.releaseKeyboard()
        QDialog.close(self)

    def emitPicked(self):
        img = self.pix.toImage()
        color = QColor(img.pixel(GRAB_RADIUS, GRAB_RADIUS))
        self.picked.emit(color)

    def keyPressEvent(self, event):
        key = event.key()
        dx = 0
        dy = 0
        if key == Qt.Key_Left:
            dx = -1
        elif key == Qt.Key_Right:
            dx = 1
        elif key == Qt.Key_Up:
            dy = -1
        elif key == Qt.Key_Down:
            dy = 1
        elif key == Qt.Key_Return:
            self.emitPicked()
            self.close()
        elif key == Qt.Key_Escape:
            self.close()
        else:
            return

        pos = QCursor.pos()
        QCursor.setPos(pos.x() + dx, pos.y() + dy)

    def updatePosition(self):
        pos = QCursor.pos()
        desktopRect = QApplication.desktop().availableGeometry(pos)

        newPos = QPoint()
        if pos.x() + GRAB_SIZE + self.width() < desktopRect.width():
            newPos.setX(pos.x() + GRAB_SIZE)
        else:
            newPos.setX(pos.x() - GRAB_SIZE - self.width())
        if pos.y() + GRAB_SIZE + self.height() < desktopRect.height():
            newPos.setY(pos.y() + GRAB_SIZE)
        else:
            newPos.setY(pos.y() - GRAB_SIZE - self.height())

        self.move(newPos)

        wid = QApplication.desktop().winId()
        self.pix = QPixmap.grabWindow(wid, pos.x() - GRAB_SIZE / 2, pos.y() - GRAB_SIZE / 2, GRAB_SIZE, GRAB_SIZE)
        self.update()

    def paintEvent(self, event):
        painter = QPainter(self)
        pix = self.pix.scaled(GRAB_SIZE * MAGNIFY, GRAB_SIZE * MAGNIFY)
        painter.drawPixmap(0, 0, pix)

        painter.setPen(Qt.darkGray)
        rect = self.rect().adjusted(0, 0, -1, -1)
        painter.drawRect(rect)

        painter.setPen(Qt.white)
        rect = rect.adjusted(1, 1, -1, -1)
        painter.drawRect(rect)

        painter.setCompositionMode(QPainter.RasterOp_SourceXorDestination)
        painter.setPen(Qt.white)
        painter.drawRect(GRAB_RADIUS * MAGNIFY - 1, GRAB_RADIUS * MAGNIFY - 1, MAGNIFY + 1, MAGNIFY + 1)
