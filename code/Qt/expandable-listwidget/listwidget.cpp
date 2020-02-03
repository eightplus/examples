#include "listwidget.h"
#include "listitem.h"
#include "utils.h"

#include <QDebug>

ListWidget::ListWidget(QWidget *parent)
    : QListWidget(parent)
{
    this->setMouseTracking(true);
    this->setFixedSize(450, 400);
    this->setFocusPolicy(Qt::NoFocus);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionRectVisible(false);
}

void ListWidget::addItem(ListItem *item)
{
    QListWidgetItem *widgetItem = new QListWidgetItem(this);
    QListWidget::addItem(widgetItem);

    QSize size = item->sizeHint();
    size.setWidth(qMin(size.width(), width()));
    size.setHeight(ITEM_HEIGHT);
    widgetItem->setSizeHint(size);
    setItemWidget(widgetItem, item);

    if (count() == 1)
        widgetItem->setSelected(true);
}

void ListWidget::addItem(const QString &title, const QString &message, const QStringList &contents)
{
    ListItem *item = new ListItem(this);

    item->setTitle(title);
    item->setMessage(message);
    item->setDetailContents(contents);

    addItem(item);
}

//更新指定行的高度
void ListWidget::updateItemSize(QListWidgetItem *item)
{
    const ListItem *widget = static_cast<ListItem *>(itemWidget(item));
    if (widget) {
        if (widget->isDetailWidgetVisible()) {
            item->setSizeHint(QSize(widget->width(), widget->getDetailWidgetHeight() + ITEM_HEIGHT));
        }
        else {
            item->setSizeHint(QSize(widget->width(), ITEM_HEIGHT));
        }
    }
}
