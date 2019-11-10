#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "MainWindow constructor thread id:" << QThread::currentThreadId();

    this->setWindowTitle(tr("QThread"));
    this->resize(600, 454);

    m_startBtn = new QPushButton(this);
    m_startBtn->setText(tr("Start"));
    connect(m_startBtn, &QPushButton::clicked, this, [=] () {
        qDebug() << "Button clicked, thread id:" << QThread::currentThreadId();
        this->m_startBtn->setEnabled(false);
        m_textLabel->setText(tr("正在执行耗时操作，请稍等！"));
        emit this->requestDoWork();
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


    m_worker = new Worker();
    m_thread = new QThread();
    m_worker->moveToThread(m_thread);//移动worker对象实例到线程中
    connect(m_worker, SIGNAL(destroyed(QObject*)), m_thread, SLOT(quit()));//当worker对象实例销毁时，退出线程
    connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));//当线程结束时，销毁线程对象实例
    connect(m_worker, &Worker::sendCount, this, &MainWindow::onResponseCount);
    connect(this, SIGNAL(requestDoWork()), m_worker, SLOT(doLongTimeWork()));
    m_thread->start();//启动线程
}

MainWindow::~MainWindow()
{
    if (m_worker) {
        m_worker->deleteLater();
    }

    if (m_thread) {
        m_thread->quit();
        m_thread->wait(2000);
    }
}

void MainWindow::onResponseCount(int count)
{
    qDebug() << "onResponseCount: " << count << ", thread id:" << QThread::currentThreadId();
    m_textLabel->setText(tr("已完成第%1次耗时操作，点击开始按钮可以继续执行耗时操作！").arg(count + 1));
    this->m_startBtn->setEnabled(true);
}
