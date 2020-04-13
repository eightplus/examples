#include "widget.h"
//#include "native-event-filter-factory.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.installNativeEventFilter(NativeEventFilterFactory::getEventFilter());
    Widget w;
    w.show();

    return a.exec();
}
