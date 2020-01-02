#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFile>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void waitCommand();

private:
    Ui::Widget *ui;

protected:
    void paintEvent(QPaintEvent *event);

private:
    QFile qstdin;
    QPixmap m_pixmap;
};

#endif // WIDGET_H
