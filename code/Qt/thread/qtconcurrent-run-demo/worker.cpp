#include "worker.h"

#include <QFile>
#include <QDebug>
#include <QtConcurrent>
#include <QFutureWatcher>
#include <QThread>
#include <QString>
#include <unistd.h>

QList<CityInfo> load_data_from_file()
{
    //在线程中执行，所以线程id与主进程的不一样
    qDebug() << "load_data_from_file thread id:" << QThread::currentThreadId();
    QFile file("./china-city-list.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QList<CityInfo>();

    const auto &content_list = file.readAll().split('\n');
    QList<CityInfo> m_list;
    m_list.reserve(content_list.size());

    for (const QString &line : content_list)
    {
        //CN101250101,changsha,长沙,CN,China,中国,hunan,湖南,changsha,长沙,28.19409,112.98228,"430101,430100,430000",
        const QStringList &info = line.split(',');
        if (info.size() < 10)
            continue;

        if (!info[0].startsWith("CN"))
            continue;

        const QString &id = info[0];
        const QString &region = info[2];
        const QString &city = info[9];
        const QString &province = info[7];

        CityInfo unit { id, region, city, province };
        m_list << std::move(unit);

        usleep(1000);
    }

    file.close();

    return m_list;
}

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
    //QFutureWatcher<QList<CityInfo>> *watcher = new QFutureWatcher<QList<CityInfo>>();
    auto *watcher = new QFutureWatcher<QList<CityInfo>>(/*this*/);
    connect(watcher, &QFutureWatcher<QList<CityInfo>>::finished, this, &Worker::onInitFinished);
    connect(watcher, &QFutureWatcher<QList<CityInfo>>::resultReadyAt, this, &Worker::onResultReadyAt);

    /*  QtConcurrent::run()用于在另外的线程运行一个函数 */
    //QFuture<QList<CityInfo>> future = QtConcurrent::run(load_data_from_file);
//    auto future = QtConcurrent::run(load_data_from_file);
    auto future = QtConcurrent::run(this, &Worker::loadDataFromFile);//开始异步调用，QtConcurrent::run()用于在另外的线程运行一个函数
    watcher->setFuture(future);
}

void Worker::onInitFinished()
{
    static int count = 0;
    qDebug() << "Worker::onInitFinished";
    QFutureWatcher<QList<CityInfo>> *watcher = static_cast<QFutureWatcher<QList<CityInfo>> *>(sender());
    if (!watcher)
        return;

    m_cityInfos = watcher->result();
    for (const auto &info : m_cityInfos)
    {
        //qDebug() << "region:" <<  info.region;
    }

    watcher->deleteLater();

    emit scanFinished(count++);
}

void Worker::onResultReadyAt(int index)
{
    QFutureWatcher<QList<CityInfo>> *watcher = static_cast<QFutureWatcher<QList<CityInfo>> *>(sender());
    if (!watcher)
        return;

    QList<CityInfo> info_list = watcher->resultAt(index);
    qDebug() << "length: "<< info_list.length() << ", index: " << index << ", region:" << info_list.at(0).region;
}

QList<CityInfo> Worker::loadDataFromFile()
{
    //在线程中执行，所以线程id与主进程的不一样
    qDebug() << "loadDataFromFile thread id:" << QThread::currentThreadId();
    QFile file("./china-city-list.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QList<CityInfo>();

    const auto &content_list = file.readAll().split('\n');
    QList<CityInfo> m_list;
    m_list.reserve(content_list.size());

    for (const QString &line : content_list)
    {
        //CN101250101,changsha,长沙,CN,China,中国,hunan,湖南,changsha,长沙,28.19409,112.98228,"430101,430100,430000",
        const QStringList &info = line.split(',');
        if (info.size() < 10)
            continue;

        if (!info[0].startsWith("CN"))
            continue;

        const QString &id = info[0];
        const QString &region = info[2];
        const QString &city = info[9];
        const QString &province = info[7];

        CityInfo unit { id, region, city, province };
        m_list << std::move(unit);

        usleep(1000);
    }

    file.close();

    return m_list;
}
