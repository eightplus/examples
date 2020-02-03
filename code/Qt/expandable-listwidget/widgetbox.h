#ifndef WIDGETBOX_H
#define WIDGETBOX_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QFrame;
class QVBoxLayout;
QT_END_NAMESPACE

class WidgetBox : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetBox(QWidget *parent = 0);

    void setWidget(QWidget *widget);
    QWidget *widget() const;

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

private:
    QWidget *m_widget = nullptr;
    QFrame *m_frame = nullptr;
    QVBoxLayout *m_layout = nullptr;
};

#endif // WIDGETBOX_H
