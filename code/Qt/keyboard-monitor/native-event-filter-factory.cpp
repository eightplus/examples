#include "native-event-filter-factory.h"

#include <QByteArray>
#include <QAbstractNativeEventFilter>
#include <QString>
#include <QObject>
#include <QDebug>

#include <X11/XKBlib.h>
#include <xcb/xcb.h>
#include <xcb/xproto.h> //xcb_button_release_event_t  xcb_button_release_event_t  xcb_key_press_event_t


class NativeEventFilter : public QAbstractNativeEventFilter {
public:

    NativeEventFilter() : entered(false) {


    }

    bool nativeEventFilter(const QByteArray& eventType,
                           void* message,
                           long*) override {
        QString event_type = QString(eventType);

        if (event_type == "xcb_generic_event_t") {//XCB(Linux) 这里只处理了键盘事件XCB_KEY_PRESS
//        if (eventType.compare("xcb_generic_event_t") == 0) {
            xcb_generic_event_t* ev = static_cast<xcb_generic_event_t*>(message);

            int evType = (ev->response_type & ~0x80);
//            qDebug() << "xcb_generic_event_t===========" << evType;

            if (evType == XCB_ENTER_NOTIFY) {
                qDebug() << "XCB_ENTER_NOTIFY===========";
                entered = true;
            }
            else if (evType == XCB_LEAVE_NOTIFY) {
                qDebug() << "XCB_LEAVE_NOTIFY===========";
                entered = false;
            }
            else if (evType == XCB_BUTTON_PRESS) {
                qDebug() << "XCB_BUTTON_PRESS===========";
            }
        }
        return false;
    }

    /*NativeEventFilter *getInstance(void)
    {
        static NativeEventFilter m_instance;
        return &m_instance;


//        const NativeEventFilter* m_instance = static_cast<const NativeEventFilter*>();
//        return const_cast<NativeEventFilter*>(m_instance);
    }*/

private:
    bool entered;
};


QAbstractNativeEventFilter* NativeEventFilterFactory::getEventFilter()
{
//    return NativeEventFilter::getInstance();
    NativeEventFilter *m_instance = new NativeEventFilter();
    return m_instance;
}
