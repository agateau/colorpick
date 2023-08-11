#include <QApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

#include "window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);

    QTranslator qtTranslator, translator;

    // For some reason Qtc loads the wrong locale, force it for debugging.
#if 1
    QLocale locale = QLocale::system();
#else
    QLocale locale(QLocale("it"));
    QLocale::setDefault(locale);
#endif
    // install the translations built-into Qt itself
    if (qtTranslator.load(QStringLiteral("qt_") + locale.name(),
#if QT_VERSION < 0x060000
                          QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
#else
                          QLibraryInfo::path(QLibraryInfo::TranslationsPath)))
#endif
        app.installTranslator(&qtTranslator);

    // E.g. "<appname>_en"
    QString translationsFileName = QCoreApplication::applicationName().toLower() + '_'
                                   + locale.name();
    // Try first in the same binary directory, in case we are building,
    // otherwise read from system data
    QString translationsPath = QCoreApplication::applicationDirPath();

    bool isLoaded = translator.load(translationsFileName, translationsPath);
    if (!isLoaded) {
        // "/usr/share/<appname>/translations
        isLoaded = translator.load(translationsFileName,
                                   QStringLiteral(PROJECT_DATA_DIR)
                                       + QStringLiteral("/translations"));
    }
    if (isLoaded)
        app.installTranslator(&translator);

    auto iconName = QString(":/icons/sc-apps-colorpick.svg");
    app.setWindowIcon(QIcon(iconName));

    Window window;
    window.show();
    return app.exec();
}
