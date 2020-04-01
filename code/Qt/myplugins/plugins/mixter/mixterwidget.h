#ifndef MIXTERWIDGET_H
#define MIXTERWIDGET_H

#include <QWidget>
#include <QLabel>

class MixterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MixterWidget(QWidget *parent = nullptr);

private:
    QLabel *m_label;
};

#endif // MIXTERWIDGET_H
