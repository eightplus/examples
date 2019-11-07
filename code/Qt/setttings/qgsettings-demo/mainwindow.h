#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QButtonGroup;
class Preferences;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void bindConnections();
    void loadConfData();

public slots:
    void onAgeValueChanged(int value);
    void onRadioButtonClicked();

private:
    Ui::MainWindow *ui;
    QButtonGroup *m_buttonGroup = nullptr;
    Preferences *m_pref = nullptr;
};

#endif // MAINWINDOW_H
