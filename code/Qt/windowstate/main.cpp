#include "widget.h"
#include <QApplication>

//sudo apt install libqt5x11extras5-dev libx11-dev,

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    //TODO: read stdin

    return a.exec();
}
