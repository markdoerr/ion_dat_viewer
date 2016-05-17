#include "mainwindow.h"

#include <QApplication>

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);

    MainWindow viewer_window;
    viewer_window.showMaximized();

    return app.exec();
}
