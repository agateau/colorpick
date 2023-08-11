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

[1]: http://agateau.com/2014/lightweight-project-management

## Requirements

- CMake
- Qt 5
- KF5GuiAddons
- KF5WidgetsAddons

## Installation

Create a build directory and change to it:

    mkdir build
    cd build

Run CMake and build:

    cmake path/to/colorpick
    make

Install (must be run as root if installing to /usr or /usr/local):

    make install

## Translations

To contribute language translations, use Qt Linguist and/or manually:

- Copy `resource/translations/colorpick.ts` language template to
  `resource/translations/colorpick_<country-code>.ts`, e.g.: `colorpick_de.ts`
- Set the language in the file:
  from `<TS version="2.1">` to `<TS version="2.1" language="de">`
- Translate it and the `resources/freedesktop/colorpick.desktop` file by adding
  e.g.: `GenericName[de]`, `Comment[de]` and optionally `Name[de]`
  (see the existing translations as reference)
- Create a Pull Request with your changes.

## Author

Aurélien Gâteau

## License

BSD
