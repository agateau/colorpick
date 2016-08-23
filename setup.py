#!/usr/bin/env python3
# encoding: utf-8
"""
Colorpick: Color picker and contrast checker

:copyright: 2012-2014 Aurélien Gâteau.
:license: BSD.
"""

from kdistutils import setup

import colorpick

DESCRIPTION = "Color picker and contrast checker"

setup(name=colorpick.__appname__,
    version=colorpick.__version__,
    description=DESCRIPTION,
    author="Aurélien Gâteau",
    author_email="mail@agateau.com",
    license=colorpick.__license__,
    platforms=["any"],
    requires=["PyQt4", "PyKDE4"],
    url="http://github.com/agateau/colorpick",
    packages=["colorpick"],
    data_files=[
        ("share/applications/kde4", ["desktop/colorpick.desktop"]),
        ("share/doc/colorpick", ["LICENSE", "README.md"]),
    ],
    scripts=["bin/colorpick"],
    )
