#include "mainwindow.h"

#include <QApplication>
#include <QMouseEvent>
#include <QScreen>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QtDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_mousePressed(false)
{
    this->setWindowTitle(tr("Log Handler"));
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setFocusPolicy(Qt::ClickFocus);
    this->setFixedSize(700, 450);

    //安装消息处理程序
#if QT_VERSION >= 0x050000
    qInstallMessageHandler(MainWindow::myMessageOutput);
#else
    qInstallMsgHandler(MainWindow::myMessageOutput);
#endif

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setFixedSize(50, 24);
    m_closeBtn->setText(tr("Close"));
    connect(m_closeBtn, &QPushButton::clicked, this, [=] () {
        qApp->quit();
    });

    m_debugBtn = new QPushButton(this);
    m_debugBtn->setText("Debug");
    m_debugBtn->setFixedSize(150, 36);

    m_infoBtn = new QPushButton(this);
    m_infoBtn->setText("Info");
    m_infoBtn->setFixedSize(150, 36);

    m_waringBtn = new QPushButton(this);
    m_waringBtn->setText("Warning");
    m_waringBtn->setFixedSize(150, 36);

    m_criticalBtn = new QPushButton(this);
    m_criticalBtn->setText("Critical");
    m_criticalBtn->setFixedSize(150, 36);

    m_fatalBtn = new QPushButton(this);
    m_fatalBtn->setText("Fatal");
    m_fatalBtn->setFixedSize(150, 36);

    connect(m_debugBtn, &QPushButton::clicked, [=] () {
        QString filename = "debug.mp4";
        qDebug("%s", filename.toUtf8().constData());
    });

    connect(m_infoBtn, &QPushButton::clicked, [=] () {
        QString filename = "info.mp4";
        qInfo("%s", filename.toUtf8().constData());
    });

    connect(m_waringBtn, &QPushButton::clicked, [=] () {
        QString filename = "warning.mp4";
        qWarning("%s", filename.toUtf8().constData());
    });

    connect(m_criticalBtn, &QPushButton::clicked, [=] () {
        QString filename = "critical.mp4";
        qCritical("%s", filename.toUtf8().constData());
    });

    connect(m_fatalBtn, &QPushButton::clicked, [=] () {
        QString filename = "fatal.mp4";
        qFatal("%s", filename.toUtf8().constData());
    });

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setSpacing(20);
    vlayout->setMargin(0);
    vlayout->addStretch();
    vlayout->addWidget(m_debugBtn, 0, Qt::AlignHCenter);
    vlayout->addWidget(m_infoBtn, 0, Qt::AlignHCenter);
    vlayout->addWidget(m_waringBtn, 0, Qt::AlignHCenter);
    vlayout->addWidget(m_criticalBtn, 0, Qt::AlignHCenter);
    vlayout->addWidget(m_fatalBtn, 0, Qt::AlignHCenter);
    vlayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(m_closeBtn, 0, Qt::AlignTop | Qt::AlignRight);
    mainLayout->addStretch();
    mainLayout->addLayout(vlayout);
    mainLayout->addStretch();

    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    this->moveCenter();
}

MainWindow::~MainWindow()
{

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

#if QT_VERSION >= 0x050000
void MainWindow::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString & msg) {
#else
void MainWindow::myMessageOutput(QtMsgType type, const char *msg) {
#endif
    static QString m_origLine;
    static QString m_resultLine;
    static QRegExp m_regExp;

    m_regExp.setPattern(".*");

    m_resultLine.clear();

#if QT_VERSION >= 0x050000
    m_origLine = msg;
    //printf("context: %s:%u, %s)\n", context.file, context.line, context.function);
#else
    m_origLine = QString::fromUtf8(msg);
#endif

    switch (type) {
        case QtDebugMsg:
            if (m_regExp.indexIn(m_origLine) > -1) {
                fprintf(stderr, "Debug: %s\n", m_origLine.toLocal8Bit().data());
                m_resultLine = m_origLine;
            }
            break;
        case QtInfoMsg:
            fprintf(stderr, "Info: %s\n", m_origLine.toLocal8Bit().data());
            m_resultLine = "INFO: " + m_origLine;
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s\n", m_origLine.toLocal8Bit().data());
            m_resultLine = "WARNING: " + m_origLine;
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s\n", m_origLine.toLocal8Bit().data());
            m_resultLine = "CRITICAL: " + m_origLine;
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s\n", m_origLine.toLocal8Bit().data());
            m_resultLine = "FATAL: " + m_origLine;
            abort();
    }

    if (m_resultLine.isEmpty()) return;

    qDebug() << "Log:" << m_resultLine;
}
