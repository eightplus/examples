#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QPushButton;
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
    QPushButton *m_debugBtn = nullptr;
    QPushButton *m_infoBtn = nullptr;
    QPushButton *m_waringBtn = nullptr;
    QPushButton *m_criticalBtn = nullptr;
    QPushButton *m_fatalBtn = nullptr;
    QPushButton *m_closeBtn = nullptr;

    QPoint m_dragPosition;
    bool m_mousePressed;

#if QT_VERSION >= 0x050000
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &, const QString & msg);
#else
    static void myMessageOutput(QtMsgType type, const char *msg);
#endif
};

#endif // MAINWINDOW_H
