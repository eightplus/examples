#include <QObject>

class Utils : public QObject
{
public:
    static void setStayOnTop(int wid, bool top);
    static void showFullscreenWindow(int wid, bool fullscreen);
    static void showMaximizedWindow(int wid, int state);
    static void showMinimizedWindow(int wid, bool minimized);
    static void changeWinDesktop(int winId, bool b);
};
