#ifndef RESTART_WIDGET_H
#define RESTART_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class RestartWidget : public QWidget
{
    Q_OBJECT

public:
    RestartWidget(QString appName, QWidget * parent = 0);
    ~RestartWidget();

signals:
    void requestCloseWindow();
    void requestRestartApp();

private:
    QLabel *m_msgLabel = nullptr;
    QPushButton *m_closeBtn = nullptr;
//    QPushButton *m_restartBtn = nullptr;
    QString m_appName;
};

#endif // RESTART_WIDGET_H
