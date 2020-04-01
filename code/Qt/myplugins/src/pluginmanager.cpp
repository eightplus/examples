#include "pluginmanager.h"
#include "plugininterface.h"

#include <QApplication>
#include <QDir>
#include <QLibrary>
#include <QPluginLoader>
#include <QDebug>

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{

}

void PluginManager::loadPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
    pluginsDir.cd("../plugins");
    const QStringList plugins = pluginsDir.entryList(QDir::Files);
    for (const QString &file : plugins) {
        if (!QLibrary::isLibrary(file))
            continue;

        const QString &filePath = pluginsDir.absoluteFilePath(file);
        QPluginLoader loader(filePath, this);
        PluginInterface *pi = qobject_cast<PluginInterface *>(loader.instance());
        if (!pi) {
            qDebug() << "Error: " << loader.errorString();
            loader.unload();
            continue;
        }

        pi->init();

        QWidget *w = pi->centralWidget();
        emit pluginAdded(w);
    }

    emit finished();
}
