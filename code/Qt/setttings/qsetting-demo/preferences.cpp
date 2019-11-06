#include "preferences.h"

#include <QDebug>
#include <QDir>
#include <QSettings>

Preferences::Preferences(QObject *parent) :
    QObject(parent)
{
    QString filename = QDir::homePath() + "/.config/eightplus/qsettings-demo.ini";
    m_settings = new QSettings(filename, QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");

    reset();
}

Preferences::~Preferences()
{
    save();

    if (m_settings) {
        delete m_settings;
        m_settings = nullptr;
    }
}

void Preferences::reset()
{
    m_name = "lixiang";
    m_male = true;
    m_age = 18;
}

void Preferences::save()
{
    m_settings->beginGroup("General");
    m_settings->setValue("name", m_name);
    m_settings->setValue("male", m_male);
    m_settings->setValue("age", m_age);
    m_settings->endGroup();
    m_settings->sync();
}

void Preferences::load()
{
    m_settings->beginGroup("General");
    m_name = m_settings->value("name", m_name).toString();
    if (m_name.isEmpty()) {
        m_name = "lixiang";
    }
    m_male = m_settings->value("male", m_male).toBool();
    m_age = m_settings->value("age", m_age).toInt();
    m_settings->endGroup();

    emit this->send_string(ValueType::NameType, m_name);
    emit this->send_int(ValueType::AgeType, m_age);
    emit this->send_bool(ValueType::SexType, m_male);
}

void Preferences::set_bool_value(const QString &key, bool value)
{
    qDebug() << "set_bool_value" << key << value;
    if (key == "male") {
        m_male = value;
    }
    m_settings->beginGroup("General");
    m_settings->setValue(key, value);
    m_settings->endGroup();
    m_settings->sync();
}

void Preferences::set_int_value(const QString &key, int value)
{
    qDebug() << "set_int_value" << key << value;
    if (key == "age") {
        m_age = value;
    }
    m_settings->beginGroup("General");
    m_settings->setValue(key, value);
    m_settings->endGroup();
    m_settings->sync();
}

void Preferences::set_string_value(const QString &key, QString value)
{
    qDebug() << "set_string_value" << key << value;
    if (key == "name") {
        m_name = value;
    }
    m_settings->beginGroup("General");
    m_settings->setValue(key, value);
    m_settings->endGroup();
    m_settings->sync();
}
