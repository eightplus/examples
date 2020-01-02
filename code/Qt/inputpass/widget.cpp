#include "widget.h"
#include "ui_widget.h"
#include "utils.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
    , m_pixmap(QPixmap(":/res/writing.png"))
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->setGeometry(0, 0, qApp->desktop()->width(), qApp->desktop()->height());
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnBottomHint | Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground, true);//设置背景透明
    this->setWindowState(Qt::WindowNoState | Qt::WindowFullScreen);
    this->setFocusPolicy(Qt::NoFocus);
    this->setWindowOpacity(0.6);

    Utils::passInputEvent(this->winId());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::waitCommand()
{
    qDebug() << "Attentions: input set to set passinput\n   input unset to cancel passinput";
    // open stdin for reading
    qstdin.open(stdin, QIODevice::ReadOnly);

    while (true) {
        printf("Input your command: ");

        QString line = QString::fromLocal8Bit(qstdin.readLine()).trimmed();
        if (line.isEmpty()) {
            continue;
        }
        else if (line == "set") {
            Utils::passInputEvent(this->winId());
        }
        else if (line == "unset") {
            Utils::unPassInputEvent(this->winId(), this->size());
        }
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    qreal ratio = qApp->devicePixelRatio();
    QPoint iconPoint = QPoint(this->width() / ratio - m_pixmap.width(), this->height() / ratio - m_pixmap.height());//右下角
    painter.setOpacity(1);
    painter.drawPixmap(iconPoint, m_pixmap);

    QString text = "Hello, World!";
    QRectF textRect(iconPoint.x(), iconPoint.y(), m_pixmap.width(), m_pixmap.height());
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    painter.setOpacity(1);
    painter.setPen(QPen(QColor("#000000")));
    painter.drawText(textRect, Qt::AlignCenter, text);
}
