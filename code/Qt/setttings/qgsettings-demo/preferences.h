#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QObject>
#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QGSettings;
QT_END_NAMESPACE

class Preferences : public QObject
{
    Q_OBJECT
public:
    enum ValueType { NoType = 0, NameType = 1, AgeType = 2, SexType = 2};
    explicit Preferences(QObject *parent = 0);
    ~Preferences();

    void load_data();
    void reset_value(const QString &key);
    bool get_bool_value(const QString &key);
    void set_bool_value(const QString &key, bool value);
    int get_int_value(const QString &key);
    void set_int_value(const QString &key, int value);
    QString get_string_value(const QString &key);
    void set_string_value(const QString &key, QString value);

signals:
    void send_string(ValueType type, const QString &value);
    void send_int(ValueType type, int value);
    void send_bool(ValueType type, bool value);

private:
    QGSettings *m_gsettings = nullptr;
};

#endif // PREFERENCES_H
