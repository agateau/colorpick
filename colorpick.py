#!/usr/bin/env python
# encoding: utf-8
import signal
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

# As per http://www.w3.org/TR/WCAG20/#visual-audio-contrast
ACCEPTABLE_CONTRAST_RATIO = 3.
GOOD_CONTRAST_RATIO = 4.5

class ColorWidget(QWidget):
    colorChanged = pyqtSignal(QColor)

    def __init__(self, parent = None):
        QWidget.__init__(self, parent)

        self.colorButton = KColorButton(self)
        self.colorButton.setFixedWidth(48)

        self.edit = QLineEdit()

        self.darkerButton = QPushButton(i18n("Darker"))
        self.lighterButton = QPushButton(i18n("Lighter"))

        self.luminanceLabel = QLabel()

        self.layout = QGridLayout(self)
        self.layout.setMargin(0)
        self.layout.addWidget(self.colorButton, 0, 0)
        self.layout.addWidget(self.edit, 0, 1)
        self.layout.addWidget(self.darkerButton, 0, 2)
        self.layout.addWidget(self.lighterButton, 0, 3)
        self.layout.addWidget(self.luminanceLabel, 1, 0, 1, 4)

        self.colorButton.changed.connect(self.slotColorChanged)
        self.edit.textChanged.connect(self.slotTextChanged)
        self.darkerButton.clicked.connect(self.darken)
        self.lighterButton.clicked.connect(self.lighten)
        self.color = QColor()
        self.luminance = 0

    def darken(self):
        self.setColor(self.color.darker(120))

    def lighten(self):
        self.setColor(self.color.lighter(120))

    def setColor(self, color):
        self.edit.setText(color.name())

    def slotTextChanged(self, txt):
        color = QColor(txt)
        if color != self.color:
            self.colorButton.setColor(color)

    def slotColorChanged(self, color):
        self.color = color
        self.edit.setText(color.name())
        self.luminance = KColorUtils.luma(color)
        self.updateLuminanceLabel()
        self.colorChanged.emit(color)

    def updateLuminanceLabel(self):
        txt = i18n("Luminance: %1", KGlobal.locale().formatNumber(self.luminance, 3))
        self.luminanceLabel.setText(txt)

class Window(KMainWindow):
    def __init__(self):
        KMainWindow.__init__(self)
        self.bgColorWidget = ColorWidget()
        self.fgColorWidget = ColorWidget()
        self.ratioLabel = QLabel()
        self.demoLabel = QLabel(i18n("Hello World"))
        self.demoLabel.setAutoFillBackground(True)

        mainWidget = QWidget()
        self.setCentralWidget(mainWidget)
        self.layout = QVBoxLayout(mainWidget)
        self.layout.addWidget(self.bgColorWidget)
        self.layout.addWidget(self.fgColorWidget)
        self.layout.addWidget(self.ratioLabel)
        self.layout.addWidget(self.demoLabel)

        self.bgColorWidget.colorChanged.connect(self.slotColorChanged)
        self.fgColorWidget.colorChanged.connect(self.slotColorChanged)

    def slotColorChanged(self):
        self.updateRatio()
        self.updateDemoLabel()

    def updateDemoLabel(self):
        pal = QPalette()
        pal.setColor(QPalette.Window, self.bgColorWidget.color)
        pal.setColor(QPalette.WindowText, self.fgColorWidget.color)
        self.demoLabel.setPalette(pal)

    def updateRatio(self):
        ratio = KColorUtils.contrastRatio(self.bgColorWidget.color, self.fgColorWidget.color)
        if ratio < ACCEPTABLE_CONTRAST_RATIO:
            level = i18n("Bad")
        elif ratio < GOOD_CONTRAST_RATIO:
            level = i18n("Acceptable")
        else:
            level = i18n("Good")
        txt = i18n("Contrast Ratio: %1:1 (%2)", KGlobal.locale().formatNumber(ratio, 2), level)
        self.ratioLabel.setText(txt)

def main():
    signal.signal(signal.SIGINT, signal.SIG_DFL)

    aboutData = KAboutData(
        "colorpick", # appName
        "", # catalogName
        ki18n("Colorpick"), # programName
        "1.0")
    aboutData.setLicense(KAboutData.License_GPL_V3)
    aboutData.setShortDescription(ki18n("Color picker and contrast checker"))
    aboutData.setCopyrightStatement(ki18n("(c) 2012 Aurélien Gâteau"))
    aboutData.setProgramIconName("fill-color")

    KCmdLineArgs.init(sys.argv, aboutData)
    app = KApplication()
    window = Window()

    window.show()
    app.exec_()
    return 0

if __name__ == "__main__":
    sys.exit(main())
# vi: ts=4 sw=4 et
