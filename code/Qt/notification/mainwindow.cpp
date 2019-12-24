#include "mainwindow.h"

#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QTimer>

#include <QtDBus>
#include <QDBusConnection>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_mousePressed(false)
    , m_notifyOK(false)
    , m_currentId(0)
{
    this->setWindowTitle(tr("Notification"));
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setFixedSize(700, 450);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFixedSize(50, 24);
    m_closeBtn->setText(tr("Close"));
    connect(m_closeBtn, &QPushButton::clicked, this, [=] () {
        qApp->quit();
    });

    m_testBtn = new QPushButton(this);
    m_testBtn->setText("Test notification1");
    m_testBtn->setFixedSize(150, 36);

    m_testBtn2 = new QPushButton(this);
    m_testBtn2->setText("Test notification2");
    m_testBtn2->setFixedSize(150, 36);

    m_notifyInterface = new QDBusInterface("org.freedesktop.Notifications",
                        "/org/freedesktop/Notifications","org.freedesktop.Notifications",
                        QDBusConnection::sessionBus());
    if (m_notifyInterface->lastError().type() != QDBusError::NoError) {
        m_notifyOK = false;
        qWarning() << "Create notification dbus interface failed";
    }
    else {
        m_notifyOK = true;
    }

    connect(m_testBtn, &QPushButton::clicked, [=] () {
        QDBusInterface notification("org.freedesktop.Notifications",
                                    "/org/freedesktop/Notifications",
                                    "org.freedesktop.Notifications",
                                    QDBusConnection::sessionBus());

        QList<QVariant> args;
        args << (QCoreApplication::applicationName())    // appname
            << ((unsigned int) 0)                       // id
            << QString("dialog-information")            // icon
            << tr("Test notification")                  // summary (notification title)
            << tr("Hello, My name is lixiang")          // body
            << QStringList()                            // actions
            << QVariantMap()                            // hints
            << (int) -1;                                // timeout
        notification.callWithArgumentList(QDBus::AutoDetect, "Notify", args);
    });

    connect(m_testBtn2, &QPushButton::clicked, [=] () {
        if (!m_notifyOK)
            return;

        QList<QVariant> args;
        args.append(QCoreApplication::applicationName());   // appname
        args.append((unsigned int) m_currentId);            // id
        args.append(QString("dialog-warning"));             // icon
        args.append(tr("Test notification"));               // summary (notification title)
        args.append(tr("Hello, My name is lixiang"));       // body
        args.append(QStringList());                         // actions
        args.append(QVariantMap());                         // hints
        //QVariantMap hints;
        //hints.insert("image_path", "kylin-video");          // image_path will replace icon
        //args.append(hints);                                 // hints
        args.append(0);                                     // timeout

        QDBusReply<uint> reply = m_notifyInterface->callWithArgumentList(QDBus::Block, "Notify", args);
        if(reply.isValid()) {
            m_currentId = reply.value();
        }
    });

    QHBoxLayout *hlayout = new QHBoxLayout();
    hlayout->setSpacing(50);
    hlayout->setMargin(0);
    hlayout->addStretch();
    hlayout->addWidget(m_testBtn, 0, Qt::AlignVCenter);
    hlayout->addWidget(m_testBtn2, 0, Qt::AlignVCenter);
    hlayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
    mainLayout->addStretch();
    mainLayout->addLayout(hlayout);
    mainLayout->addStretch();

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    this->moveCenter();
}

MainWindow::~MainWindow()
{
    m_notifyInterface->deleteLater();
}

void MainWindow::moveCenter()
{
    QPoint pos = QCursor::pos();
    QRect primaryGeometry;
    for (QScreen *screen : qApp->screens()) {
        if (screen->geometry().contains(pos)) {
            primaryGeometry = screen->geometry();
        }
    }

    if (primaryGeometry.isEmpty()) {
        primaryGeometry = qApp->primaryScreen()->geometry();
    }

    this->move(primaryGeometry.x() + (primaryGeometry.width() - this->width())/2,
               primaryGeometry.y() + (primaryGeometry.height() - this->height())/2);
    this->show();
    this->raise();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        this->m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        this->m_mousePressed = true;
    }

    QWidget::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    this->m_mousePressed = false;

    QWidget::mouseReleaseEvent(event);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (this->m_mousePressed) {
        move(event->globalPos() - this->m_dragPosition);
    }

    QWidget::mouseMoveEvent(event);
}
