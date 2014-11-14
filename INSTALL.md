Since Colorpick is a KDE application, it must be installed in KDE installation
dir.  If you do not install it in this dir, Colorpick won't be able to load
data files and its icon won't appear in launchers.

To do so, you have to run as root:

    python setup.py install --prefix=`kde4-config --prefix`

This won't work on Ubuntu however. Instead, you should use:

    sudo python setup.py install --install-layout=deb
