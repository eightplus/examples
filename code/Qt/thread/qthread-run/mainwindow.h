#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "workerthread.h"

QT_BEGIN_NAMESPACE
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onResponseCount(int count);

private:
    QPushButton *m_startBtn = nullptr;
    QLabel *m_textLabel = nullptr;
    WorkerThread m_thread;
};

#endif // MAINWINDOW_H
