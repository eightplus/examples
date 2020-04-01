#include "mixterwidget.h"

extern "C" {

#include <glib.h>
#include <libmatemixer/matemixer.h>

}

#include <QVBoxLayout>

MixterWidget::MixterWidget(QWidget *parent)
    : QWidget(parent)
    , m_label(new QLabel)
{
    m_label->setAlignment(Qt::AlignHCenter);
    m_label->setText("hello, kobe");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_label);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    if (mate_mixer_init() == false)
        m_label->setText("matemixer init failed");
    else
        m_label->setText("matemixer init success");
}
