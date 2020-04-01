#include "contentwidget.h"
#include "pluginmanager.h"

#include <QStackedLayout>
#include <QTimer>

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_pluginsLayout(new QStackedLayout)
{
    this->setLayout(m_pluginsLayout);
}

ContentWidget::~ContentWidget()
{

}

void ContentWidget::initPlugins()
{
    PluginManager *pluginManager = new PluginManager;
    connect(pluginManager, &PluginManager::finished, pluginManager, &PluginManager::deleteLater);
    connect(pluginManager, &PluginManager::pluginAdded, this, &ContentWidget::onPluginAdded);

    QTimer::singleShot(1, pluginManager, &PluginManager::loadPlugins);
}

void ContentWidget::onPluginAdded(QWidget * const w)
{
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    const int index = m_pluginsLayout->addWidget(w);
    m_pluginsLayout->setCurrentIndex(index);
}
