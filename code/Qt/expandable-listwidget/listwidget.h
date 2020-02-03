#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

QT_BEGIN_NAMESPACE
class ListItem;
QT_END_NAMESPACE

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);

    void addItem(ListItem *item);
    void addItem(const QString &title, const QString &message, const QStringList &contents);
    void updateItemSize(QListWidgetItem *item);
};

#endif // LISTWIDGET_H
