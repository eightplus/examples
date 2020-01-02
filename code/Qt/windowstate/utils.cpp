#include "utils.h"

#include <QDebug>
#include <QtX11Extras/QX11Info>
#include <X11/extensions/shape.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

// https://tronche.com/gui/x/xlib/window-information/XInternAtom.html
// https://linux.die.net/man/3/xinternatom
// https://tronche.com/gui/x/xlib/event-handling/XSendEvent.html
// https://tronche.com/gui/x/xlib/events/structures.html
// https://tronche.com/gui/x/xlib/events/client-communication/client-message.html#XClientMessageEvent
// https://tronche.com/gui/x/xlib/window-information/properties-and-atoms.html

namespace {

unsigned char *getWindowProperty(Display *display, Window win, const char *prop)
{
    Atom reqAtom = XInternAtom(display, prop, true);
    if (reqAtom == None) {
        return NULL;
    }

    int retCheck = None;
    Atom retType = None;
    int retFormat = 0;
    unsigned long retItems = 0UL;
    unsigned long retMoreBytes = 0UL;
    unsigned char* retValue = NULL;

    retCheck = XGetWindowProperty(display, win, reqAtom, 0L, 0L, false, AnyPropertyType,
        &retType, &retFormat, &retItems, &retMoreBytes, &retValue);

    if (retValue != NULL) {
        XFree(retValue);
        retValue = NULL;
    }

    if (retCheck != Success || retType == None || retMoreBytes == 0) {
        return NULL;
    }

    retCheck = None;
    retFormat = 0;
    retItems = 0UL;

    // Convert the byte length into 32bit multiples.
    if (retMoreBytes % 4 != 0)
        retMoreBytes += 4 - retMoreBytes % 4;
    retMoreBytes /= 4;

    // Request the actual property value with correct length and type.
    retCheck = XGetWindowProperty(display, win, reqAtom, 0L, retMoreBytes, false, retType,
        &retType, &retFormat, &retItems, &retMoreBytes, &retValue);

    if (retCheck != Success || retMoreBytes != 0) {
        if (retValue != NULL)
            XFree(retValue);
        return NULL;
    }

    return retValue;
}

}

// 设置窗口置顶/取消窗口置顶
void Utils::setStayOnTop(int wid, bool top)
{
    const auto display = QX11Info::display();//Display *display = QX11Info::display();

    const auto wmState = XInternAtom(display, "_NET_WM_STATE", false);
    const auto wmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", false);
    const auto wmStateStaysOnTop = XInternAtom(display, "_NET_WM_STATE_STAYS_ON_TOP", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.xclient.type = ClientMessage;
    xev.xclient.display = display;
    xev.xclient.window = wid;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    if (top)
        xev.xclient.data.l[0] = 1;// add/set property
    else
        xev.xclient.data.l[0] = 0;// remove/unset property
    xev.xclient.data.l[1] = wmStateAbove;
    xev.xclient.data.l[2] = wmStateStaysOnTop;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
        QX11Info::appRootWindow(QX11Info::appScreen()),
        false,
        SubstructureRedirectMask | SubstructureNotifyMask,
        &xev);

    XFlush(display);
}

void Utils::showFullscreenWindow(int wid, bool fullscreen)
{
    const auto display = QX11Info::display();//Display *display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const Atom wmState = XInternAtom(display, "_NET_WM_STATE", false);
    const Atom wmfullscreenState = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.xclient.type = ClientMessage;
    xev.xclient.display = display;
    xev.xclient.window = wid;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    if (fullscreen) {
        xev.xclient.data.l[0] = 1;//add/set property
    } else {
        xev.xclient.data.l[0] = 0;//remove/unset property
    }
    xev.xclient.data.l[1] = wmfullscreenState;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );
    XFlush(display);
}

void Utils::showMaximizedWindow(int wid, int state)
{
    // state:
    //      0: remove/unset property
    //      1: add/set property
    //      2: toggle property
    const auto display = QX11Info::display();//Display *display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const Atom wmState = XInternAtom(display, "_NET_WM_STATE", false);
    const Atom wmAddState = XInternAtom(display, "_NET_WM_STATE_ADD", false);
    const Atom wmVMaximizedState = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", false);
    const Atom wmHMaximizedState = XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.xclient.type = ClientMessage;
    xev.xclient.display = display;
    xev.xclient.window = wid;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = state;//xev.xclient.data.l[0] = wmAddState;
    xev.xclient.data.l[1] = wmVMaximizedState;
    xev.xclient.data.l[2] = wmHMaximizedState;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev);
    XFlush(display);
}

void Utils::showMinimizedWindow(int wid, bool minimized)
{
    const auto display = QX11Info::display();//Display *display = QX11Info::display();
    const auto screen = QX11Info::appScreen();

    const Atom wmState = XInternAtom(display, "_NET_WM_STATE", false);
    const Atom wmHiddenState = XInternAtom(display, "_NET_WM_STATE_HIDDEN", false);

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.xclient.type = ClientMessage;
    xev.xclient.display = display;
    xev.xclient.window = wid;
    xev.xclient.message_type = wmState;
    xev.xclient.format = 32;
    if (minimized) {
        xev.xclient.data.l[0] = 1;//add/set property
    } else {
        xev.xclient.data.l[0] = 0;//remove/unset property
    }
    xev.xclient.data.l[1] = wmHiddenState;
    xev.xclient.data.l[2] = 0;
    xev.xclient.data.l[3] = 1;

    XSendEvent(display,
               QX11Info::appRootWindow(screen),
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );
    XIconifyWindow(display, wid, screen);
    XFlush(display);
}

void Utils::changeWinDesktop(int wid, bool b)
{
    unsigned long desktop = ~(0UL);

    const auto display = QX11Info::display();//Display *display = QX11Info::display();
    const auto screen = QX11Info::appScreen();
    Window root = QX11Info::appRootWindow(screen);//Window root = DefaultRootWindow(display);

    const Atom wmDesktopState = XInternAtom(display, "_NET_WM_DESKTOP", false);

    if (!b) {
        unsigned char *tmp = getWindowProperty(display, root, "_NET_CURRENT_DESKTOP");
        if (tmp == NULL) {
            qWarning("error reading current desktop property.");
        }
        else {
            desktop = *(reinterpret_cast<unsigned long*>(tmp));
            XFree(tmp);
        }
    }

    XEvent xev;
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.type = ClientMessage;
    xev.xclient.display = display;
    xev.xclient.window = wid;
    xev.xclient.message_type = wmDesktopState;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = desktop;

    XSendEvent(display,
               root,
               false,
               SubstructureRedirectMask | SubstructureNotifyMask,
               &xev
              );
    XFlush(display);
}
