#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
//#include <QThread>
#include <QPointer>

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
    void onResponseFinished(int count);

signals:
    void requestDoWork();

private:
    QPushButton *m_startBtn = nullptr;
    QLabel *m_textLabel = nullptr;
    QPointer<Worker> m_worker;
};

#endif // MAINWINDOW_H
