#include "core.h"
#include <QDir>
#include <QFileInfo>
#include <QRegExp>

#include "videowindow.h"
#include "myprocess.h"
#include "preferences.h"
#include "global.h"

using namespace Global;

Core::Core(VideoWindow *mpw, QWidget* parent)
	: QObject( parent ) 
{
	qRegisterMetaType<Core::State>("Core::State");

    m_playerWindow = mpw;
	_state = Stopped;
	we_are_restarting = false;
    proc = new MyProcess(this);

    connect(proc, SIGNAL(receivedCurrentSec(double)), this, SLOT(changeCurrentSec(double)));
    connect(proc, SIGNAL(receivedCurrentFrame(int)), this, SIGNAL(showFrame(int)));
    connect(proc, SIGNAL(receivedPause()), this, SLOT(changePause()));
    connect(proc, SIGNAL(processExited()), this, SLOT(processFinished()), Qt::QueuedConnection);
    connect(proc, SIGNAL(playerFullyLoaded()), this, SLOT(finishRestart()), Qt::QueuedConnection);
    connect(proc, SIGNAL(receivedCacheMessage(QString)), this, SLOT(displayMessage(QString)));
    connect(proc, SIGNAL(receivedBuffering()), this, SIGNAL(buffering()));
    connect(proc, SIGNAL(receivedPlaying()), this, SLOT(displayPlaying()));
    connect(proc, SIGNAL(receivedCacheEmptyMessage(QString)), this, SIGNAL(buffering()));
    connect(proc, SIGNAL(receivedCreatingIndex(QString)), this, SLOT(displayMessage(QString)) );
    connect(proc, SIGNAL(receivedCreatingIndex(QString)), this, SIGNAL(buffering()));
    connect(proc, SIGNAL(receivedConnectingToMessage(QString)), this, SLOT(displayMessage(QString)));
    connect(proc, SIGNAL(receivedConnectingToMessage(QString)), this, SIGNAL(buffering()));
    connect(proc, SIGNAL(receivedResolvingMessage(QString)), this, SLOT(displayMessage(QString)));
    connect(proc, SIGNAL(receivedResolvingMessage(QString)), this, SIGNAL(buffering()));
    connect(proc, SIGNAL(receivedScanningFont(QString)), this, SLOT(displayMessage(QString)) );
    connect(proc, SIGNAL(receivedWindowResolution(int,int)), this, SLOT(gotWindowResolution(int,int)));
    connect(proc, SIGNAL(receivedNoVideo()), this, SLOT(gotNoVideo()));
    connect(proc, SIGNAL(receivedVO(QString)), this, SLOT(gotVO(QString)));
    connect(proc, SIGNAL(receivedAO(QString)), this, SLOT(gotAO(QString)));
    connect(proc, SIGNAL(receivedEndOfFile()), this, SLOT(fileReachedEnd()), Qt::QueuedConnection );
    connect(proc, SIGNAL(receivedStreamTitle(QString)), this, SLOT(streamTitleChanged(QString)));
    connect(proc, SIGNAL(receivedStreamTitleAndUrl(QString,QString)), this, SLOT(streamTitleAndUrlChanged(QString,QString)));
	connect(this, SIGNAL(buffering()), this, SLOT(displayBuffering()));
    mset.reset();
}


Core::~Core()
{
    if (proc->isRunning()) stopPlayer();
	proc->terminate();
	delete proc;
}

void Core::setState(State s)
{
	if (s != _state) {
		_state = s;
        emit stateChanged(_state);
        if (_state == Stopped) {
            mset.current_sec = 0;
            emit showTime(mset.current_sec, true);
            emit positionChanged(0);
        }
	}
}


void Core::restart()
{
	if (proc->isRunning()) {
		restartPlay();
    }
    else {
        qDebug() << "Core::restart: mpv is not running";
	}
}

void Core::updateWidgets()
{
	emit widgetsNeedUpdate();
}

void Core::openFile(QString filename, int seek)
{
	QFileInfo fi(filename);
	if (fi.exists()) {
        if (proc->isRunning()) {
            stopPlayer();
            we_are_restarting = false;
        }

        mdat.reset();
        mdat.m_filename = fi.absoluteFilePath();

        int old_volume = mset.volume;
        mset.reset();

        mset.volume = old_volume;
        initPlaying(seek);
    }
}

void Core::restartPlay(int seek)
{
	we_are_restarting = true;
    initPlaying(seek);
}

void Core::initPlaying(int seek)
{
	if (proc->isRunning()) {
        stopPlayer();
	}

	int start_sec = (int) mset.current_sec;
	if (seek > -1) start_sec = seek;

    startPlayer(mdat.m_filename, start_sec);
}

void Core::newMediaPlaying()
{
    QString file = mdat.m_filename;
	mdat = proc->mediaData();
    mdat.m_filename = file;
	mset.current_demuxer = mdat.demuxer;
}

void Core::finishRestart()
{
	if (!we_are_restarting) {
		newMediaPlaying();
		emit mediaStartPlay();
	} 

	if (we_are_restarting) {
		mdat.demuxer = proc->mediaData().demuxer;
	}

	we_are_restarting = false;

    int vol = (pref->global_volume ? pref->volume : mset.volume);
    volumeChanged(vol);

	emit newDuration(mdat.duration);

    emit mediaDataReceived(mdat);

    updateWidgets();
}

void Core::stop()
{
    if (state() == Stopped) {
		qDebug("Core::stop: mset.current_sec: %f", mset.current_sec);
		mset.current_sec = 0;
		qDebug("Core::stop: mset.current_sec set to 0");
        emit showTime(mset.current_sec, true);
        emit positionChanged(0);
	}

    stopPlayer();

    mset.current_sec = 0;
    emit showTime( mset.current_sec, true);
    emit positionChanged(0);
}

void Core::play(int seek)
{
	if ((proc->isRunning()) && (state()==Paused)) {
		proc->setPause(false);
	}
	else
	if ((proc->isRunning()) && (state()==Playing)) {
	}
	else {
        if ( !mdat.m_filename.isEmpty() ) {
            restartPlay(seek);
        }
	}
}

void Core::pause()
{
	if (proc->isRunning()) {
		if (state() == Paused) proc->setPause(false); else proc->setPause(true);
	}
}

void Core::playOrPause(int seek) {
	if (proc->isRunning()) {
		pause();
	} else {
        play(seek);
	}
}

void Core::processFinished()
{
	if (!we_are_restarting) {
		setState(Stopped);
	}

	int exit_code = proc->exitCode();
	if (exit_code != 0) {
        setState(Stopped);
        emit stateChanged(Stopped);
	}
}

void Core::fileReachedEnd()
{
	mset.current_sec = 0;
	updateWidgets();
}

void Core::goToPosition(int value)
{
    if (mdat.duration > 0) {
        int jump_time = (int) mdat.duration * value / 1000;
        goToSec(jump_time);
    }
}

void Core::goToPos(double perc)
{
	seek_cmd(perc, 1);
}

void Core::startPlayer(QString file, double seek)
{
	if (file.isEmpty()) {
        qWarning("Core:startPlayer: file is empty!");
		return;
	}

	if (proc->isRunning()) {
        qWarning("Core::startPlayer: Player still running!");
		return;
    }

	proc->clearArguments();
    proc->setExecutable("/usr/bin/mpv");
	proc->setFixedOptions();
    proc->setOption("fs", false);

	if (!mset.forced_demuxer.isEmpty()) {
		proc->setOption("demuxer", mset.forced_demuxer);
	}

	else{
        proc->setOption("vc", "coreserve,");//−vc <[-]编解码器1,[-]编解码器2,...[,]>   设置可用编解码器的优先级列表, 按照它们在codecs.conf中的编解码器 名称. 在名称前加’-’表示忽略该编解码器
    }
    if (!pref->vo.isEmpty()) {
        QString vo = pref->vo;
        if (!vo.endsWith(",")) vo += ",";
        proc->setOption("vo", vo);
    }

    if (!pref->ao.isEmpty()) {
        QString ao = pref->ao;
        proc->setOption("ao", ao);
    }

    proc->disableInput();
    proc->setOption("keepaspect", false);

    proc->setOption("wid", QString::number( (qint64) m_playerWindow->displayLayer()->winId() ) );//将视频输出定位到widget窗体部件中,-wid参数只在X11、directX和OpenGL中适用

    proc->setOption("monitorpixelaspect", "1");

    int vol = (pref->global_volume ? pref->volume : mset.volume);
    vol = adjustVolume(vol, pref->use_soft_vol ? pref->softvol_max : 100);
    proc->setOption("volume", QString::number(vol));

	if (pref->mute) {
		proc->setOption("mute");
	}

    proc->setOption("cache_auto");

    if (pref->use_soft_vol) {
        proc->setOption("softvol", QString::number(pref->softvol_max));
    } else {
        proc->setOption("softvol", "off");
    }

    proc->setMedia(file, false);

	if (mset.loop) {
		proc->setOption("loop", "0");
	}

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

    proc->setProcessEnvironment(env);
    if (!proc->start()) {
        qDebug() << "mpv process didn't start";
    }
}

void Core::stopPlayer()
{
	if (!proc->isRunning()) {
        qWarning("Core::stopPlayer: player is not running!");
		return;
	}

	proc->quit();

    qDebug("Core::stopPlayer: Waiting player to finish...");
    if (!proc->waitForFinished(5000)) {
		proc->kill();
	}

    qDebug("Core::stopPlayer: Finished. (I hope)");
}

void Core::goToSec(double sec)
{
	if (sec < 0) sec = 0;
	if (sec > mdat.duration ) sec = mdat.duration - 20;
	seek_cmd(sec, 2);
}

void Core::seek(int secs)
{
	if ( (proc->isRunning()) && (secs!=0) ) {
		seek_cmd(secs, 0);
	}
}

void Core::seek_cmd(double secs, int mode)
{
    proc->seek(secs, mode, true);
}

void Core::forward(int secs)
{
    seek(secs);
}

void Core::rewind(int secs)
{
    seek(-secs);
}

void Core::wheelUp()
{
    forward(30);
}

void Core::wheelDown()
{
    rewind(30);
}

int Core::adjustVolume(int v, int max_vol)
{
	if (max_vol < 100) max_vol = 100;
	int vol = v * max_vol / 100;
	return vol;
}

void Core::setVolume(int volume, bool force)
{
	int current_volume = (pref->global_volume ? pref->volume : mset.volume);

	if ((volume == current_volume) && (!force)) return;

	current_volume = volume;
	if (current_volume > 100) current_volume = 100;
	if (current_volume < 0) current_volume = 0;


    int vol = adjustVolume(current_volume, pref->use_soft_vol ? pref->softvol_max : 100);
    proc->setVolume(vol);

	if (pref->global_volume) {
		pref->volume = current_volume;
        if (pref->volume <= 0) {
            pref->mute = true;
        }
        else {
            pref->mute = false;
        }
    }
    else {
		mset.volume = current_volume;
        if (mset.volume <= 0) {
            mset.mute = true;
        }
        else {
            mset.mute = false;
        }
	}

	updateWidgets();

	displayMessage( tr("Volume: %1").arg(current_volume) );
	emit volumeChanged( current_volume );
}

int Core::getVolumn()
{
    return pref->volume;
}

bool Core::getMute()
{
    return (pref->global_volume ? pref->mute : mset.mute);
}

void Core::mute(bool b)
{
	proc->mute(b);

	if (pref->global_volume) {
		pref->mute = b;
	} else {
		mset.mute = b;
	}

	updateWidgets();
}

void Core::incVolume(int step)
{
    int vol = pref->global_volume ? pref->volume : mset.volume;
    setVolume(vol + step);
}

void Core::decVolume(int step)
{
    int vol = pref->global_volume ? pref->volume : mset.volume;
    setVolume(vol - step);
}

void Core::incVolume()
{
    incVolume(4);
}

void Core::decVolume()
{
    decVolume(4);
}

void Core::changeCurrentSec(double sec)
{
    mset.current_sec = sec;

	if (state() != Playing) {
		setState(Playing);
	}

    emit showTime(mset.current_sec, false);

	static int last_second = 0;

    if (floor(sec)==last_second) return;
	last_second = (int) floor(sec);

	int value = 0;
    if ((mdat.duration > 1) && (mset.current_sec > 1) && (mdat.duration > mset.current_sec))
	{
        value = ( (int) mset.current_sec * 1000) / (int) mdat.duration;
	}
    emit positionChanged(value);
}

void Core::changePause()
{
	setState(Paused);
    emit stateChanged(state());
}

void Core::displayMessage(QString text)
{
    emit showMessage(text);
}

void Core::displayBuffering()
{
    setState(Buffering);
    emit showMessage(tr("Buffering..."));
}

void Core::displayPlaying()
{
    setState(Buffering);
    emit showMessage(tr("Starting..."), 60000);
}

void Core::gotWindowResolution(int w, int h)
{

    emit needResize(w,h);
    mset.win_width = w;
    mset.win_height = h;

    m_playerWindow->setResolution( w, h );
}

void Core::gotNoVideo()
{
	emit noVideo();
}

void Core::gotVO(QString vo)
{
    if (pref->vo.isEmpty()) {
		pref->vo = vo;
	}
}

void Core::gotAO(QString ao)
{
    if (pref->ao.isEmpty()) {
        pref->ao = ao;
    }
}

void Core::streamTitleChanged(QString title)
{
	mdat.stream_title = title;
}

void Core::streamTitleAndUrlChanged(QString title, QString url)
{
	mdat.stream_title = title;
}

void Core::sendMediaInfo()
{
    emit mediaPlaying(mdat.m_filename, mdat.displayName(true));
}
