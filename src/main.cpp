#include <QApplication>

#include "window.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(app);
    auto iconName = QString(":/icons/sc-apps-colorpick.svg");
    app.setWindowIcon(QIcon(iconName));
    Window window;
    window.show();
    return app.exec();
}
