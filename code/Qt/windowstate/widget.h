#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
QT_END_NAMESPACE

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;

private:
    QPushButton *m_stayontopBtn;
    QPushButton *m_unstayontopBtn;
    QPushButton *m_fullScreenBtn;
    QPushButton *m_maxBtn;
    QPushButton *m_toggleMaxBtn;
    QPushButton *m_normalBtn;
    QPushButton *m_minBtn;
    QPushButton *m_desktopBtn;
    QPushButton *m_closeBtn;
};

#endif // WIDGET_H
