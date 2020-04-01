#include "mixterplugin.h"

MixterPlugin::MixterPlugin()
{

}

void MixterPlugin::init()
{
    Q_ASSERT(m_centralWidget.isNull());
    m_centralWidget = new MixterWidget;
}
