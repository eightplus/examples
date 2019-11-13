#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtConcurrent>
#include <QFutureWatcher>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QGridLayout;
class QPushButton;
class QLabel;
class QProgressBar;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initConections();

public slots:
    void onMapBtnClicked();
    void onMappedBtnClicked();
    void onMappedReducedBtnClicked();
    void onMappedReducedBtn2Clicked();

    void onOpenBtnClicked();
    void showImage(int index);
    void finished();
    void onSetProgressRange(int minimum, int maximum);
    void onProgressValueChanged(int value);

private:
    QPushButton *m_openBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;
    QPushButton *m_pauseBtn = nullptr;

    QPushButton *m_mapBtn = nullptr;
    QPushButton *m_mappedBtn = nullptr;
    QPushButton *m_mappedReducedBtn = nullptr;
    QPushButton *m_mappedReducedBtn2 = nullptr;

    QVBoxLayout *m_mainLayout = nullptr;
    QGridLayout *m_imagesLayout = nullptr;
    QList<QLabel *> m_labels;
    QProgressBar *m_progressBar = nullptr;
    QFutureWatcher<QImage> *m_watcher = nullptr;
};

#endif // MAINWINDOW_H
