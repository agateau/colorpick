#!/usr/bin/env python
# encoding: utf-8
import signal
import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

from colorwidget import ColorWidget

from ui_mainwindow import Ui_MainWindow

# As per http://www.w3.org/TR/WCAG20/#visual-audio-contrast
ACCEPTABLE_CONTRAST_RATIO = 3.
GOOD_CONTRAST_RATIO = 4.5


class Window(KMainWindow, Ui_MainWindow):
    def __init__(self):
        KMainWindow.__init__(self)

        mainWidget = QWidget()
        self.setCentralWidget(mainWidget)
        self.setupUi(mainWidget)

        self.bgColorWidget.colorChanged.connect(self.slotColorChanged)
        self.fgColorWidget.colorChanged.connect(self.slotColorChanged)
        self.updateRatio()

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
