#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ContentWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    ContentWidget *m_contentWidget;
};

#endif // MAINWINDOW_H
