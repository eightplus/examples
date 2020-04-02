#pragma once

#include <QAbstractNativeEventFilter>

class NativeEventFilterFactory {
public:
    static QAbstractNativeEventFilter *getEventFilter();
};
