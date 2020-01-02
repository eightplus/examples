#include "widget.h"
#include <QApplication>

//sudo apt install libqt5x11extras5-dev libx11-dev libxext-dev

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
