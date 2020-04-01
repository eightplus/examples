#ifndef CONTENTWIDGET_H
#define CONTENTWIDGET_H

#include "plugininterface.h"

#include <QWidget>

class QStackedLayout;

class ContentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ContentWidget(QWidget *parent = nullptr);
    ~ContentWidget();

public slots:
    void onPluginAdded(QWidget * const w);
    void initPlugins();

private:
    QStackedLayout *m_pluginsLayout;
};

#endif // CONTENTWIDGET_H
