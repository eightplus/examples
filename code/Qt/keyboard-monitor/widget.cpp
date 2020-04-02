#include "widget.h"

#include <QEvent>
#include <QGuiApplication>
#include <QInputMethodEvent>
#include <QInputMethod>
#include <QTransform>
#include <QPoint>
#include <QCursor>
#include <QClipboard>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>

#include <X11/XKBlib.h>
//解决QEvent::Type中enum和Xlib中#defines同名的冲突，比如Qt中的QEvent::KeyPress与Xlib中的#define KeyPress 2（定义在X.h文件中）
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_lineEdit(new QLineEdit(this))
    , m_label(new QLabel(this))
{
    this->setAttribute(Qt::WA_InputMethodEnabled);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFixedSize(400, 300);
    installEventFilter(this);

    m_lineEdit->setEchoMode(QLineEdit::Password);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &Widget::onLineEditTextChanged);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(20);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_label);
    this->setLayout(layout);

    checkStatus();
}

Widget::~Widget()
{

}

// http://stackoverflow.com/questions/2968336/qt-password-field-warn-about-caps-lock
bool Widget::checkStatusStatus()
{
    Display* d = XOpenDisplay((char*)0);
    bool caps_state = false;
    if (d) {
        unsigned n;
        XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
        caps_state = (n & 0x01) == 1;
    }
    return caps_state;
}

void Widget::checkStatus()
{
    if (checkStatusStatus()) {
        m_label->setText("Caps Lock is on");
    }
    else {
        m_label->setText("Caps Lock is off");
    }
}

void Widget::onLineEditTextChanged(const QString &text)
{
    Q_UNUSED(text);

    checkStatus();
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::InputMethod) {//让QLineEdit失去焦点后进行输入法测试
        QInputMethodEvent* e = (QInputMethodEvent*) event;
        if(!hasFocus()) {//失去焦点时，输入法事件优先于FocusOut事件被处理
            e->setCommitString("");
        }
        else {
            qDebug() << "InputMethod preeditString:" << e->preeditString() << ", commitString:" << e->commitString();
            QInputMethod* im = QGuiApplication::inputMethod();
            QTransform t;
            //QPoint p((0 - this->width()) / 2 + 20, 0 - this->height());
            QPoint p(0, 0);
            QCursor cursor;
            QPoint position(this->mapToGlobal(QPoint(20, 0)));
            cursor.setPos(position);
            this->setCursor(cursor);
            t.translate(p.x(), p.y());

            QGuiApplication::inputMethod()->setInputItemTransform(t);
            QGuiApplication::inputMethod()->setInputItemRectangle(this->rect());
            QGuiApplication::inputMethod()->update(Qt::ImQueryInput);

    //        qDebug() << "anchor        " << im->anchorRectangle();
    //        qDebug() << "cursor        " << im->cursorRectangle();
    //        qDebug() << "direction     " << im->inputDirection();
    //        qDebug() << "itemclip      " << im->inputItemClipRectangle();
    //        qDebug() << "itemrectangle " << im->inputItemRectangle();
    //        qDebug() << "animating     " << im->isAnimating();
    //        qDebug() << "visible       " << im->isVisible();
    //        qDebug() << "keyboard      " << im->keyboardRectangle();
    //        qDebug() << this->inputMethodQuery(Qt::ImAnchorPosition);
    //        qDebug() << this->inputMethodQuery(Qt::ImAnchorRectangle);
        }
    }
    else if (event->type() == QEvent::KeyPress) {
        //qDebug() << "QEvent::KeyPress";
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if (keyEvent) {
            qDebug() << "keyEvent->text():" << keyEvent->text() << ",key():" << keyEvent->key();

            if (keyEvent->key() == Qt::Key_Meta) {//windows key
                qDebug() << "Key_Meta";
            }
            else if (keyEvent->key() == Qt::Key_Home) {
                qDebug() << "Key_Home";
            }
            else if (keyEvent->key() == Qt::Key_CapsLock) {
                qDebug() << "Key_CapsLock";
            }
            else if (keyEvent->key() == Qt::Key_NumLock) {
                qDebug() << "Key_NumLock";
            }
            else if (keyEvent->key() == Qt::Key_Left) {
                qDebug() << "Key_Left";
            }
            else if (keyEvent->key() == Qt::Key_Right) {
                qDebug() << "Key_Right";
            }
            else if (keyEvent->key() == Qt::Key_Up) {
                qDebug() << "Key_Up";
            }
            else if (keyEvent->key() == Qt::Key_Down) {
                qDebug() << "Key_Down";
            }
            else if (keyEvent->key() == Qt::Key_End) {
                qDebug() << "Key_End";
            }
            else if (keyEvent->key() == Qt::Key_7) {
                qDebug() << "7";
            }
            else if (keyEvent->key() == Qt::Key_4) {
                qDebug() << "4";
            }
            else if (keyEvent->key() == Qt::Key_6) {
                qDebug() << "6";
            }
            else if (keyEvent->key() == Qt::Key_8) {
                qDebug() << "8";
            }
            else if (keyEvent->key() == Qt::Key_2) {
                qDebug() << "2";
            }
            else if (keyEvent->key() == Qt::Key_1) {
                qDebug() << "1";
            }
        }
    }
    else if (event->type() == QEvent::KeyRelease) {
        //qDebug() << "QEvent::KeyRelease";
    }
    else if (event->type() == QEvent::MouseButtonPress) {
        qDebug() << "QEvent::MouseButtonPress";
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        qDebug() << "QEvent::MouseButtonRelease";
    }
    else if (event->type() == QEvent::MouseMove) {
        qDebug() << "QEvent::MouseMove";
    }
    else if (event->type() == QEvent::FocusIn) {
        //qDebug() << "QEvent::FocusIn";
    }
    else if (event->type() == QEvent::FocusOut) {
        //qDebug() << "QEvent::FocusOut";
    }
    return QWidget::eventFilter(watched, event);
}
