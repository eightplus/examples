#include "widgetbox.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QDebug>

WidgetBox::WidgetBox(QWidget *parent)
    : QWidget(parent)
{
    m_frame = new QFrame(this);
    m_layout = new QVBoxLayout(m_frame);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *main_layout = new QVBoxLayout(this);
    main_layout->addWidget(m_frame);
}

void WidgetBox::setWidget(QWidget *widget)
{
    if (m_widget) {
        m_layout->removeWidget(m_widget);
        m_widget->deleteLater();
    }

    m_widget = widget;
    m_layout->addWidget(widget, 0, Qt::AlignLeft | Qt::AlignTop);
    m_widget->setFixedSize((m_frame->rect() - m_layout->contentsMargins()).size());
}

QWidget *WidgetBox::widget() const
{
    return m_widget;
}

void WidgetBox::resizeEvent(QResizeEvent *event)
{
    if (m_widget)
        m_widget->setFixedSize((m_frame->rect() - m_layout->contentsMargins()).size());

    return QWidget::resizeEvent(event);
}
