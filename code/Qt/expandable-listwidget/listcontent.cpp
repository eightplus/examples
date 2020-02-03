#include "listcontent.h"
#include "listwidget.h"
#include "listitem.h"
#include "widgetbox.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QDebug>

ListContent::ListContent(QWidget *parent)
    : QWidget(parent)
    , m_widgetBox(new WidgetBox(this))
{
    m_listWidget = new ListWidget(this);
    m_listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QStringList data1;
    data1 << "aa1" << "aa2" << "aa3";
    QStringList data2;
    data2 << "bb1" << "bb2" << "bb3" << "bb4" << "bb5" << "bb6" << "bb7" << "bb8" << "bb9" << "bb10";
    QStringList data3;
    data3 << "cc1" << "cc2" << "cc3" << "cc4" << "cc5" << "cc6" << "cc7" << "cc8";
    QStringList data4;
    data4 << "dd1" << "dd2" << "dd3" << "dd4" << "dd5";
    m_listWidget->addItem(tr("aaa"), tr("AAAAAAAAAAAAAAAAAAAA"), data1);
    m_listWidget->addItem(tr("bbb"), tr("test title message first second third fourth"), data2);
    m_listWidget->addItem(tr("ccc"), tr("CCCCCCCCC"), data3);
    m_listWidget->addItem(tr("ddd"), tr("DDDDDD"), data4);

    connect(m_listWidget, &ListWidget::itemClicked, this, [=] (QListWidgetItem *item) {
        int index = m_listWidget->row(item);

        QString title;
        if (m_listWidget->selectedItems().isEmpty())
            title = QString();

        if (ListItem *m_item = qobject_cast<ListItem*>(m_listWidget->itemWidget(m_listWidget->selectedItems().first()))) {
            title = m_item->title();
            m_item->setDetailWidgetVisible();
            m_listWidget->updateItemSize(item);
        }

        qDebug () << "clicked index: " << index << ", title: " << title;
    });


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addSpacing(10);
    layout->addWidget(m_widgetBox);
    layout->addSpacing(10);

    m_widgetBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_widgetBox->setWidget(m_listWidget);
}
