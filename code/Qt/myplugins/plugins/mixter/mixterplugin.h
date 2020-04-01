#ifndef MIXTER_PLUGIN_H
#define MIXTER_PLUGIN_H

#include <QPointer>

#include "../../src/plugininterface.h"
#include "mixterwidget.h"

class MixterPlugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "com.eightplus.Plugin.PluginInterface")

public:
    explicit MixterPlugin();

    void init();
    QWidget *centralWidget() { return m_centralWidget.data(); }
    const QString name() const { return tr("Mixter"); }

private:
    QPointer<MixterWidget> m_centralWidget;
};

#endif // MIXTER_PLUGIN_H
