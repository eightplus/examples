#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "MainWindow constructor thread id:" << QThread::currentThreadId();
    connect(&m_thread, &WorkerThread::sendCount, this, &MainWindow::onResponseCount);
    connect(&m_thread, &WorkerThread::finished, this, [=] () {
        qDebug() << "thread finished........";
        m_thread.deleteLater();
    });

    this->setWindowTitle(tr("QThread"));
    this->resize(600, 454);

    m_startBtn = new QPushButton(this);
    m_startBtn->setText(tr("Start"));
    connect(m_startBtn, &QPushButton::clicked, this, [=] () {
        qDebug() << "Button clicked, thread id:" << QThread::currentThreadId();
        this->m_startBtn->setEnabled(false);
        m_textLabel->setText(tr("正在执行耗时操作，请稍等！"));
        m_thread.doLongTimeWork();
    });
    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setText(tr("请点击开始按钮开启线程去执行耗时操作！"));

    QVBoxLayout *m_layout = new QVBoxLayout();
    m_layout->addStretch();
    m_layout->addWidget(m_startBtn);
    m_layout->addWidget(m_textLabel);
    m_layout->addStretch();
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(10, 10, 10, 10);
    this->setLayout(m_layout);
}

MainWindow::~MainWindow()
{
    if (m_thread.isRunning()) {
        m_thread.deleteLater();
    }
}

void MainWindow::onResponseCount(int count)
{
    qDebug() << "onResponseCount: " << count << ", thread id:" << QThread::currentThreadId();
    m_textLabel->setText(tr("已完成第%1次耗时操作，点击开始按钮可以继续执行耗时操作！").arg(count + 1));
    this->m_startBtn->setEnabled(true);
}
