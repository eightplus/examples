#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>
#include <QFrame>

class QPushButton;
class QLabel;
class TimeSlider;
class QSlider;
class QVBoxLayout;

class BottomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BottomWidget(QWidget *parent = 0);
    ~BottomWidget();

signals:
    void requestVolumeChanged(int volume);
    void toggleStop();
    void togglePlayPause();
    void toggleMute();
    void valueChanged(int);
    void posChanged(int);
    void wheelUp();
    void wheelDown();

public slots:
    void onMusicPlayed();
    void onMusicPause();
    void onMusicStoped();
    void onVolumeChanged(int volume);
    void onMutedChanged(bool muted, int volumn);
    void displayTime(QString cur_time, QString all_time);
    void setPos(int);
    void setDuration(double);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_playtimeLabel = nullptr;
    QLabel *m_totaltimeLabel = nullptr;
    QPushButton *m_btnStop = nullptr;
    QPushButton *m_btnPlayPause = nullptr;
    QPushButton *m_btnSound = nullptr;
    TimeSlider *m_timeProgress = nullptr;
    QWidget *m_controlWidget = nullptr;
    QSlider *m_volSlider = nullptr;
    QFrame *m_metaWidget = nullptr;
    QFrame *m_ctlWidget = nullptr;
    QWidget *m_actWidget = nullptr;
    double m_totalTime;
    QVBoxLayout *vboxlayout = nullptr;
};

#endif // BOTTOMWIDGET_H
