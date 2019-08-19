#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include "core.h"
#include "global.h"
#include "preferences.h"
using namespace Global;

class QPushButton;
class QWidget;
class VideoWindow;
class QLabel;
class QAction;
class BottomWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createMenu();
    void createVideoWindow();
    void createCore();
    void createBottomToolBar();

public slots:
    void onMute();
    void onCloseWindow();
    void onPlayPause();
    void openFile();
    void openFile(QString file);
    void displayState(Core::State state);
    void displayMessage(QString message);
    void gotCurrentTime(double, bool);
    void newMediaLoaded();
    void togglePlayAction(Core::State);
    void updateWidgets();
    void resizeMainWindow(int w, int h);

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    QVBoxLayout *m_centralLayout = nullptr;
    BottomWidget *m_bottomToolbar = nullptr;
    Core *m_core = nullptr;
    VideoWindow *m_playerWindow = nullptr;
    QMenu *m_mainMenu = nullptr;
    QAction *m_openAct = nullptr;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
