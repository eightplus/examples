#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_worker(new Worker(this))
{
    qDebug() << "MainWindow constructor thread id:" << QThread::currentThreadId();

    this->setWindowTitle(tr("QtConcurrent::run"));
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

    connect(m_worker, &Worker::scanFinished, this, &MainWindow::onResponseFinished);
    connect(this, &MainWindow::requestDoWork, m_worker, &Worker::doLongTimeWork);
}

MainWindow::~MainWindow()
{
//    if (m_worker) {
//        m_worker->deleteLater();
//    }
}

void MainWindow::onResponseFinished(int count)
{
    qDebug() << "onResponseFinished: " << count << ", thread id:" << QThread::currentThreadId();
    m_textLabel->setText(tr("已完成第%1次耗时操作，点击开始按钮可以继续执行耗时操作！").arg(count + 1));
    this->m_startBtn->setEnabled(true);
}
