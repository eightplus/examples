#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QWidget>

class PluginInterface
{
public:
    virtual ~PluginInterface() {}

    virtual void init() = 0;
    virtual QWidget *centralWidget() = 0;
    virtual const QString name() const = 0;
};

QT_BEGIN_NAMESPACE

#define ModuleInterface_iid "com.eightplus.Plugin.PluginInterface"
Q_DECLARE_INTERFACE(PluginInterface, ModuleInterface_iid)

QT_END_NAMESPACE

#endif // PLUGININTERFACE_H
