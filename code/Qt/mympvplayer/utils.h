#ifndef UTILS_H
#define UTILS_H

#include <Qt>
#include <QWidget>
#include <QString>

#include "preferences.h"

class Utils {
public:
    static QString formatTime(int secs);
    static void setBackgroundColor(QWidget * w, const QColor & color);
    static void setForegroundColor(QWidget * w, const QColor & color);
};

#endif // UTILS_H
