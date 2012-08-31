from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

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

