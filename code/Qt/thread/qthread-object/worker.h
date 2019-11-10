#ifndef WORKER_H
#define WORKER_H

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

public slots:
    void doLongTimeWork();

signals:
    void sendCount(int count);

private:
    void loadDataFromFile();
};

#endif // WORKER_H
