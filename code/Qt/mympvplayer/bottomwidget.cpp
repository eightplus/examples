#include "bottomwidget.h"


#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QSlider>
#include <QPainter>

#include "utils.h"
#include "timeslider.h"
#include "global.h"
#include "preferences.h"

using namespace Global;

BottomWidget::BottomWidget(QWidget *parent)
    : QWidget(parent, Qt::SubWindow)
{
    this->setMouseTracking(true);
    this->setAutoFillBackground(true);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlags(windowFlags() | Qt::SubWindow | Qt::WindowStaysOnTopHint);

    m_controlWidget = new QWidget(this);
    m_controlWidget->setFixedHeight(61);

    m_timeProgress = new TimeSlider(this);
    m_timeProgress->setMouseTracking(true);

    connect(m_timeProgress, SIGNAL(posChanged(int)), this, SIGNAL(posChanged(int)));
    connect(m_timeProgress, SIGNAL(wheelUp()), this, SIGNAL(wheelUp()));
    connect(m_timeProgress, SIGNAL(wheelDown()), this, SIGNAL(wheelDown()));
    m_timeProgress->setFixedHeight(24);
    m_timeProgress->move(0,0);

    m_playtimeLabel = new QLabel;
    m_playtimeLabel->setText("00:00:00");
    m_playtimeLabel->setFrameShape(QFrame::NoFrame);
    Utils::setBackgroundColor(m_playtimeLabel, QColor(0,0,0) );
    Utils::setForegroundColor(m_playtimeLabel, QColor(255,255,255) );
    m_playtimeLabel->setStyleSheet("QLabel{font-size:12px;color:#ffffff;}");
    m_playtimeLabel->adjustSize();
    m_totaltimeLabel = new QLabel;
    m_totaltimeLabel->setText(" / 00:00:00");
    m_totaltimeLabel->setFrameShape(QFrame::NoFrame);
    Utils::setBackgroundColor( m_totaltimeLabel, QColor(0,0,0) );
    Utils::setForegroundColor( m_totaltimeLabel, QColor(255,255,255) );
    m_totaltimeLabel->setStyleSheet("QLabel{font-size:12px;color:#808080;}");
    m_totaltimeLabel->adjustSize();
    m_btnStop = new QPushButton;
    m_btnStop->setText(tr("Stop"));
    m_btnStop->setFixedSize(40, 24);
    m_btnPlayPause = new QPushButton;
    m_btnPlayPause->setText(tr("Play"));
    m_btnPlayPause->setFixedSize(41, 41);
    m_btnSound = new QPushButton;
    m_btnSound->setFixedSize(60, 24);
    m_btnSound->setText(tr("Volumn"));
    m_volSlider = new QSlider(Qt::Horizontal);
    m_volSlider->setMinimum(0);
    m_volSlider->setMaximum(100);
    m_volSlider->setSingleStep(10);
    m_volSlider->setValue(50);
    m_volSlider->setFocusPolicy(Qt::NoFocus);
    m_volSlider->setObjectName("VolumeProgress");
    connect(this, SIGNAL(valueChanged(int)), m_volSlider, SLOT(setValue(int)));
    connect(m_btnStop, SIGNAL(released()), this, SIGNAL(toggleStop()));
    connect(m_btnPlayPause, SIGNAL(released()), this, SIGNAL(togglePlayPause()));
    connect(m_btnSound, SIGNAL(pressed()), this, SIGNAL(toggleMute()));
    connect(m_volSlider, &QSlider::valueChanged, this, [=] (int vol) {
        this->onVolumeChanged(vol);
        emit this->requestVolumeChanged(vol);
    });

    QHBoxLayout *time_layout = new QHBoxLayout();
    time_layout->setMargin(0);
    time_layout->setSpacing(2);
    time_layout->addWidget(m_playtimeLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);
    time_layout->addWidget(m_totaltimeLabel, 0, Qt::AlignLeft | Qt::AlignVCenter);

    m_metaWidget = new QFrame(this);
    m_metaWidget->setFixedHeight(61);
    QHBoxLayout *metaLayout = new QHBoxLayout(m_metaWidget);
    metaLayout->setMargin(0);
    metaLayout->setSpacing(10);
    metaLayout->addWidget(m_btnPlayPause, 0, Qt::AlignVCenter);
    metaLayout->addLayout(time_layout);

    m_ctlWidget = new QFrame(this);
    m_ctlWidget->setFixedHeight(61);
    QHBoxLayout *ctlLayout = new QHBoxLayout(m_ctlWidget);
    ctlLayout->setMargin(0);
    ctlLayout->setSpacing(30);
    ctlLayout->addWidget(m_btnStop, 0, Qt::AlignCenter);
    m_ctlWidget->adjustSize();

    QHBoxLayout *volume_layout = new QHBoxLayout();
    volume_layout->setMargin(0);
    volume_layout->setSpacing(2);
    volume_layout->addWidget(m_btnSound, 0, Qt::AlignRight | Qt::AlignVCenter);
    volume_layout->addWidget(m_volSlider, 0, Qt::AlignRight | Qt::AlignVCenter);

    m_actWidget = new QWidget(this);
    m_actWidget->setFixedHeight(61);
    QHBoxLayout *actLayout = new QHBoxLayout(m_actWidget);
    actLayout->setMargin(0);
    actLayout->setSpacing(20);
    actLayout->addLayout(volume_layout);

    QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sp.setHorizontalStretch(33);
    m_metaWidget->setSizePolicy(sp);
    m_actWidget->setSizePolicy(sp);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(10, 0, 20, 0);
    layout->setSpacing(20);
    layout->addWidget(m_metaWidget, 0, Qt::AlignLeft/* | Qt::AlignVCenter*/);
    layout->addStretch();
    layout->addWidget(m_ctlWidget, 0, Qt::AlignHCenter/*Qt::AlignCenter*/);
    layout->addStretch();
    layout->addWidget(m_actWidget, 0, Qt::AlignRight/* | Qt::AlignVCenter*/);
    m_controlWidget->setLayout(layout);

    vboxlayout = new QVBoxLayout(this);
    vboxlayout->setSpacing(0);
    vboxlayout->setContentsMargins(0, 18, 0, 2);
    vboxlayout->addWidget(m_controlWidget);

    m_btnStop->setFocusPolicy(Qt::NoFocus);
    m_btnPlayPause->setFocusPolicy(Qt::NoFocus);
    m_btnSound->setFocusPolicy(Qt::NoFocus);
    m_volSlider->setFocusPolicy(Qt::NoFocus);

    m_btnStop->setToolTip(tr("Stop"));
    m_btnPlayPause->setToolTip(tr("Play / Pause"));
    m_btnSound->setToolTip(tr("Mute"));
}

BottomWidget::~BottomWidget()
{
    if (m_metaWidget) delete m_metaWidget;
    if (m_ctlWidget) delete m_ctlWidget;
    if (m_actWidget) delete m_actWidget;
    if (m_controlWidget) delete m_controlWidget;
}

void BottomWidget::setPos(int v)
{
    bool was_blocked= m_timeProgress->blockSignals(true);
    m_timeProgress->setPos(v);
    m_timeProgress->blockSignals(was_blocked);
}

void BottomWidget::setDuration(double t)
{
    m_totalTime = t;
    m_timeProgress->setDuration(t);
}

void BottomWidget::displayTime(QString cur_time, QString all_time)
{
    m_playtimeLabel->setText(cur_time);
    m_totaltimeLabel->setText(all_time);
}

void BottomWidget::onMusicPlayed()
{
    m_btnPlayPause->setText(tr("Pause"));
}

void BottomWidget::onMusicPause()
{
    m_btnPlayPause->setText(tr("Play"));
}

void BottomWidget::onMusicStoped()
{
    m_btnPlayPause->setText(tr("Play"));
}

void BottomWidget::onVolumeChanged(int volume)
{
    QString status = "mid";
    if (volume > 77) {
        m_btnSound->setText(tr("Volume"));
        status = "high";
    } else if (volume > 33) {
        m_btnSound->setText(tr("Volume"));
        status = "mid";
    } else if (volume > 0) {
        status = "low";
        m_btnSound->setText(tr("Volume"));
    } else  {
        status = "mute";
        m_btnSound->setText(tr("Mute"));
    }

    this->m_volSlider->blockSignals(true);
    this->m_volSlider->setValue(volume);
    this->m_volSlider->blockSignals(false);
}

void BottomWidget::onMutedChanged(bool muted, int volumn)
{
    if (muted) {
        m_btnSound->setText(tr("Mute"));
        this->m_volSlider->blockSignals(true);
        this->m_volSlider->setValue(0);
        this->m_volSlider->blockSignals(false);
    }
    else {
        this->onVolumeChanged(volumn);
    }
}

void BottomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    m_timeProgress->resize(this->width(), 24);
}

void BottomWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setCompositionMode(QPainter::CompositionMode_Clear);
    p.fillRect(rect(), Qt::SolidPattern);
}
