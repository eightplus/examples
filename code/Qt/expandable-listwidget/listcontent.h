#ifndef LISTCONTENT_H
#define LISTCONTENT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class ListWidget;
class WidgetBox;
QT_END_NAMESPACE

class ListContent : public QWidget
{
    Q_OBJECT

public:
    explicit ListContent(QWidget *parent = 0);

private:
    WidgetBox *m_widgetBox = nullptr;
    ListWidget *m_listWidget = nullptr;
};

#endif // LISTCONTENT_H
