#include "worker.h"

#include <QThread>
#include <QFile>
#include <QDebug>

Worker::Worker(QObject *parent)
    : QObject(parent)
{
    qDebug() << "Worker constructor thread id:" << QThread::currentThreadId();
}

Worker::~Worker()
{

}

void Worker::doLongTimeWork()
{
    qDebug() << "doLongTimeWork thread id:" << QThread::currentThreadId();
    static int count = 0;
    this->loadDataFromFile();
    emit sendCount(count++);
}

void Worker::loadDataFromFile()
{
    QFile file("./china-city-list.csv");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString content = file.readLine();
        while (!content.isEmpty()) {
            QThread::msleep(2);
            content = file.readLine();
        }
        file.close();
    }
}
