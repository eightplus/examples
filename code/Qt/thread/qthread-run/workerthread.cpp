#include "workerthread.h"

#include <QFile>
#include <QDebug>

WorkerThread::WorkerThread(QObject *parent)
    : QThread(parent)
    , abort(false)
{
    qDebug() << "WorkerThread constructor thread id:" << QThread::currentThreadId();
}

WorkerThread::~WorkerThread()
{
    //让run可以跳出forever
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void WorkerThread::doLongTimeWork()
{
    qDebug() << "doLongTimeWork thread id:" << QThread::currentThreadId();
    QMutexLocker locker(&mutex);//构造一个局部的QMutexLocker对象并锁住互斥量，当QMutexLocker被销毁的时候，互斥量将被自动解锁(因为QMutexLocker是个局部变量，当函数返回时它就被销毁)

    if (!isRunning()) {
        start(QThread::LowPriority);
    }
    else {
        condition.wakeOne();//唤醒一个等待waitCondition的线程
    }
}

void WorkerThread::run()
{
    static int count = 0;
    forever {
        if (abort)
            return;
        qDebug() << "run thread id:" << QThread::currentThreadId();
        qDebug() << "forever 111";
        mutex.lock();
        //当处于wait状态时mutex会被暂时释放(会自动调用unlock解锁之前锁住的资源，避免造成死锁)，并阻塞在这个地方；当线程被cond.wakeOne()等唤醒时，mutex又会被重新锁定，并继续运行
        qDebug() << "forever 222";
        this->loadDataFromFile();
        emit sendCount(count++);
        condition.wait(&mutex);//QWaitCondition 用于多线程的同步，一个线程调用QWaitCondition::wait() 阻塞等待，直到另一个线程调用QWaitCondition::wake() 唤醒才继续往下执行

        mutex.unlock();
        qDebug() << "forever 333";
    }
}


void WorkerThread::loadDataFromFile()
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
