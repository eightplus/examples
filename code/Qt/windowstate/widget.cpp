#include "widget.h"
#include "ui_widget.h"
#include "utils.h"

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->resize(700, 450);
    this->setMouseTracking(true);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    m_stayontopBtn = new QPushButton(this);
    m_unstayontopBtn = new QPushButton(this);
    m_fullScreenBtn = new QPushButton(this);
    m_maxBtn = new QPushButton(this);
    m_toggleMaxBtn = new QPushButton(this);
    m_normalBtn = new QPushButton(this);
    m_minBtn = new QPushButton(this);
    m_desktopBtn = new QPushButton(this);
    m_closeBtn = new QPushButton(this);

    m_unstayontopBtn->setText("Unstay On Top");
    m_stayontopBtn->setText("Stay On Top");
    m_fullScreenBtn->setText("Full Screen");
    m_maxBtn->setText("Max Window");
    m_toggleMaxBtn->setText("Toggle Max Window");
    m_normalBtn->setText("Normal Window");
    m_minBtn->setText("Min Window");
    m_desktopBtn->setText("Desktop");
    m_closeBtn->setText("Close Window");

    QVBoxLayout *m_layout = new QVBoxLayout;
    m_layout->addStretch();
    m_layout->addWidget(m_stayontopBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_unstayontopBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_fullScreenBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_maxBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_toggleMaxBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_normalBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_minBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_desktopBtn, 0 , Qt::AlignHCenter);
    m_layout->addWidget(m_closeBtn, 0 , Qt::AlignHCenter);
    m_layout->addStretch();
    this->setLayout(m_layout);

    connect(m_stayontopBtn, &QPushButton::clicked, this, [=] {
        Utils::setStayOnTop(this->winId(), true);
    });
    connect(m_unstayontopBtn, &QPushButton::clicked, this, [=] {
        Utils::setStayOnTop(this->winId(), false);
    });
    connect(m_fullScreenBtn, &QPushButton::clicked, this, [=] () {
        static bool checked = true;
        Utils::showFullscreenWindow(this->winId(), checked);
        checked = !checked;
//        this->show();
//        this->activateWindow();
//        this->raise();
    });
    connect(m_maxBtn, &QPushButton::clicked, this, [=] {
        Utils::showMaximizedWindow(this->winId(), 1);
//        this->show();
//        this->activateWindow();
//        this->raise();
    });
    connect(m_toggleMaxBtn, &QPushButton::clicked, this, [=] {
        Utils::showMaximizedWindow(this->winId(), 2);
    });
    connect(m_normalBtn, &QPushButton::clicked, this, [=] {
        Utils::showMaximizedWindow(this->winId(), 0);
    });
    connect(m_minBtn, &QPushButton::clicked, this, [=] {
        Utils::showMinimizedWindow(this->winId(), true);
//        this->showMinimized();
    });
    connect(m_desktopBtn, &QPushButton::clicked, this, [=] {
        static bool b = true;
        Utils::changeWinDesktop(this->winId(), b);
        b = !b;
    });
    connect(m_closeBtn, &QPushButton::clicked, this, [=] {
        qApp->quit();
    });
}

Widget::~Widget()
{
    delete ui;
}
