#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QThread>

#include "worker.h"

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

signals:
    void requestDoWork();

private:
    QPushButton *m_startBtn = nullptr;
    QLabel *m_textLabel = nullptr;
    Worker *m_worker = nullptr;
    QThread *m_thread = nullptr;
};

#endif // MAINWINDOW_H
