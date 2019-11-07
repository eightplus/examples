#include "preferences.h"

#include <QDebug>
#include <QGSettings>

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
    if (QGSettings::isSchemaInstalled("apps.eightplus.qgsettings-demo")) {
        m_gsettings = new QGSettings("apps.eightplus.qgsettings-demo");

        //监听key的value是否发生了变化
        connect(m_gsettings, &QGSettings::changed, this, [=] (const QString &key) {
            if (key == "name") {
                QString name = get_string_value("name");
                emit this->send_string(ValueType::NameType, name);
            }
            else if (key == "age") {
                int age = get_int_value("age");
                emit this->send_int(ValueType::AgeType, age);
            }
            else if (key == "male") {
                bool male = get_int_value("male");
                emit this->send_bool(ValueType::SexType, male);
            }
        });
    }
}

Preferences::~Preferences()
{
    if (m_gsettings) {
        delete m_gsettings;
        m_gsettings = nullptr;
    }
}

void Preferences::load_data()
{
    if (!m_gsettings)
        return;

    const QStringList list = m_gsettings->keys();
    if (list.contains("name")) {
        QString name = get_string_value("name");
        emit this->send_string(ValueType::NameType, name);
    }
    if (list.contains("age")) {
        int age = get_int_value("age");
        emit this->send_int(ValueType::AgeType, age);
    }
    if (list.contains("male")) {
        bool male = get_int_value("male");
        emit this->send_bool(ValueType::SexType, male);
    }
}

void Preferences::reset_value(const QString &key)
{
    if (!m_gsettings)
        return;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return;
    }

    m_gsettings->reset(key);
}

bool Preferences::get_bool_value(const QString &key)
{
    if (!m_gsettings)
        return false;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return false;
    }

    const bool value = m_gsettings->get(key).toBool();

    return value;
}

void Preferences::set_bool_value(const QString &key, bool value)
{
    if (!m_gsettings)
        return;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return;
    }

    m_gsettings->set(key, value);//trySet
}

int Preferences::get_int_value(const QString &key)
{
    if (!m_gsettings)
        return -999;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return -999;
    }

    int value = m_gsettings->get(key).toInt();

    return value;
}

void Preferences::set_int_value(const QString &key, int value)
{
    if (!m_gsettings)
        return;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return;
    }

    m_gsettings->set(key, value);
}

QString Preferences::get_string_value(const QString &key)
{
    if (!m_gsettings) {
        return QString();
    }

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return QString();
    }

    QString value = m_gsettings->get(key).toString();
    return value;
}

void Preferences::set_string_value(const QString &key, QString value)
{
    if (!m_gsettings)
        return;

    const QStringList list = m_gsettings->keys();
    if (!list.contains(key)) {
        return;
    }

    m_gsettings->set(key, value);
}
