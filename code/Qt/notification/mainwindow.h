#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QPushButton;
class QDBusInterface;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void moveCenter();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPushButton *m_testBtn = nullptr;
    QPushButton *m_testBtn2 = nullptr;
    QPushButton *m_closeBtn = nullptr;
    QDBusInterface *m_notifyInterface = nullptr;
    QPoint m_dragPosition;
    bool m_mousePressed;
    bool m_notifyOK;
    unsigned int m_currentId;
};

#endif // MAINWINDOW_H
