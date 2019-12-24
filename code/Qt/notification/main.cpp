#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("eightplus");
    app.setApplicationName("notification");
    app.setApplicationVersion("0.0.1");

    MainWindow widget;
    widget.show();

    return app.exec();
}
