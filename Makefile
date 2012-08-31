all:
	pyuic4 mainwindow.ui -o ui_mainwindow.py

clean:
	-rm *.pyc ui_*.py
