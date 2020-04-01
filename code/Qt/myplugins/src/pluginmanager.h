#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>

class PluginInterface;

class PluginManager : public QObject
{
    Q_OBJECT

public:
    explicit PluginManager(QObject *parent = nullptr);

public slots:
    void loadPlugins();

signals:
    void finished() const;
    void pluginAdded(QWidget * const w);
};

#endif // PLUGINSMANAGER_H
