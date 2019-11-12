#ifndef WORKER_H
#define WORKER_H

#include <QObject>

#include "cityinfo.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);
    ~Worker();

public slots:
    void doLongTimeWork();
    void onInitFinished();
    void onResultReadyAt(int index);

signals:
    void scanFinished(int count) const;

private:
    QList<CityInfo> m_cityInfos;
    QList<CityInfo> loadDataFromFile();
};

#endif // WORKER_H
