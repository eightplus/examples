#ifndef CITY_INFO_H
#define CITY_INFO_H

#include <QMetaType>

class CityInfo
{
public:
    QString id;
    QString region;
    QString city;
    QString province;
};
Q_DECLARE_METATYPE(CityInfo)

#endif // CITY_INFO_H
