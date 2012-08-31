from PyQt4.QtCore import *
from PyQt4.QtGui import *

from PyKDE4.kdecore import *
from PyKDE4.kdeui import *

class ColorEditor(QWidget):
    changed = pyqtSignal(QColor)

    def __init__(self, parent = None):
        QWidget.__init__(self, parent)
        layout = QGridLayout(self)
        layout.setMargin(0)
        self.selectors = []

        lst = i18nc("red", "R"), i18nc("green", "G"), i18nc("blue", "B")
        for row, comp in enumerate(lst):
            label = QLabel(comp)

            selector = KGradientSelector()
            selector.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.Minimum)
            selector.setArrowDirection(Qt.UpArrow)
            rgb = 0xFF << ((2 - row) * 8)
            selector.setColors(Qt.black, QColor(rgb))
            selector.setMaximum(255)

            spinBox = QSpinBox()
            spinBox.setMaximum(255)

            layout.addWidget(label, row, 0)
            layout.addWidget(selector, row, 1)
            layout.addWidget(spinBox, row, 2)

            spinBox.valueChanged.connect(selector.setValue)
            selector.valueChanged.connect(spinBox.setValue)
            selector.valueChanged.connect(self.emitChanged)

            self.selectors.append(selector)

    def color(self):
        return QColor(*[x.value() for x in self.selectors])

    def setColor(self, color):
        self.selectors[0].setValue(color.red())
        self.selectors[1].setValue(color.green())
        self.selectors[2].setValue(color.blue())

    def emitChanged(self):
        self.changed.emit(self.color())
