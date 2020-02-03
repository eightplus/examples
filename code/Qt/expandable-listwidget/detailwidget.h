#ifndef DETAILWIDGET_H
#define DETAILWIDGET_H

#include <QFrame>

QT_BEGIN_NAMESPACE
class QScrollArea;
class QVBoxLayout;
class QTimer;
QT_END_NAMESPACE

class DetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DetailWidget(QWidget *parent = nullptr);
    ~DetailWidget();

    void loadData(const QStringList &data);

private slots:
    void updateHeight();

private:
    bool eventFilter(QObject *, QEvent *event) Q_DECL_OVERRIDE;

private:
    QScrollArea *m_scrollArea = nullptr;
    QFrame *m_scrollWidget = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QTimer *m_timer = nullptr;
};

#endif // DETAILWIDGET_H
