# Colorpick: Color picker and contrast checker

![Contrast check](screenshots/contrast.png)

Colorpick is a color picker, which makes it easy to check text readability by
letting you pick a background and foreground color and computing the contrast
between them.

It lets you know if contrast is good enough according to
<http://www.w3.org/TR/WCAG20/#visual-audio-contrast>.

It also comes with a handy magnified-picker, which you can control with the
cursor keys, for precise picking.

Finally, it lets you adjust colors and copy them to the clipboard as different
formats.

Colorpick is managed using the [lightweight project management policy][1].

Currently, working only under X11 based systems.

## Requirements

- CMake
- Qt 5/6
- [KGuiAddons][2]
- [KWidgetsAddons][3]

## Installation

Create a build directory and change to it:

```bash
mkdir build
cd build
```

By default, Qt5 is used for build, otherwise change it by adding `-DPROJECT_QT_VERSION=6`
to the following CMake configure command:

```bash
cmake path/to/colorpick
make
```

Install (must be run as root if installing to /usr or /usr/local):

```bash
make install
```

## Author

Aurélien Gâteau

## License

BSD


[1]: http://agateau.com/2014/lightweight-project-management
[2]: https://invent.kde.org/frameworks/kguiaddons/
[3]: https://invent.kde.org/frameworks/kwidgetsaddons/
