#include "utils.h"

QString Utils::formatTime(int secs)
{
    bool negative = (secs < 0);
    secs = qAbs(secs);

    int t = secs;
    int hours = (int) t / 3600;
    t -= hours*3600;
    int minutes = (int) t / 60;
    t -= minutes*60;
    int seconds = t;

    return QString("%1%2:%3:%4").arg(negative ? "-" : "").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

void Utils::setBackgroundColor(QWidget * w, const QColor & color) {
    QPalette p = w->palette();
    p.setColor(w->backgroundRole(), color);
    w->setPalette(p);
}

void Utils::setForegroundColor(QWidget * w, const QColor & color) {
    QPalette p = w->palette();
    p.setColor(w->foregroundRole(), color);
    w->setPalette(p);
}
