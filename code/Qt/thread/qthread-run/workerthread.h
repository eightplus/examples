#ifndef WORKERTHREAD_H
#define WORKERTHREAD_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class WorkerThread : public QThread
{
    Q_OBJECT

public:
    WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();

    void doLongTimeWork();

signals:
    void sendCount(int count);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    void loadDataFromFile();

    QMutex mutex;
    QWaitCondition condition;
    bool abort;
};

#endif // WORKERTHREAD_H
