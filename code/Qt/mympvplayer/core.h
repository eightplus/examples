#ifndef _CORE_H_
#define _CORE_H_

#include <QObject>
#include <QProcess>

#include "mediadata.h"
#include "mediasettings.h"

class MyProcess;
class VideoWindow;

class Core : public QObject
{
    Q_OBJECT
    
public:
    enum State { Stopped = 0, Playing = 1, Paused = 2, Buffering = 3 };

    Core(VideoWindow *mpw, QWidget* parent = 0);
	~Core();

	MediaData mdat;
    MediaSettings mset;

	State state() { return _state; };

protected:
	void setState(State s);
    void seek_cmd(double secs, int mode);
    
public slots:
	void openFile(QString filename, int seek=-1);
    void stop(); 
    void play(int seek = -1);
    void playOrPause(int seek = -1);
	void pause();
	void restart();
	void goToPosition( int value );
	void goToPos( double perc );
	void goToSec( double sec );
	void goToSec(int sec) { goToSec( (double) sec); }
	void seek(int secs);
	void wheelUp();
	void wheelDown();
	void setVolume(int volume, bool force = false);
	void mute(bool b);
    void incVolume(int step);
    void decVolume(int step);
    void incVolume();
    void decVolume();
    bool getMute();
    int getVolumn();
    void forward(int secs);
    void rewind(int secs);

protected slots:
    void changeCurrentSec(double sec);
    void changePause();
	void gotWindowResolution( int w, int h );
	void gotNoVideo();
	void gotVO(QString);
    void gotAO(QString);
	void finishRestart();
    void processFinished();
	void fileReachedEnd();

	void displayMessage(QString text);
	void displayBuffering();
	void displayPlaying();
	void streamTitleChanged(QString);
	void streamTitleAndUrlChanged(QString,QString);
	void sendMediaInfo();

protected:
    void restartPlay(int seek=-1);
	void initPlaying(int seek=-1);
	void newMediaPlaying();
    void startPlayer(QString file, double seek = -1 );
    void stopPlayer();
	void updateWidgets();
	int adjustVolume(int v, int max_vol);

signals:
	void buffering();
    void mediaDataReceived(const MediaData &);
	void mediaPlaying(const QString & filename, const QString & title);
    void stateChanged(Core::State state);
	void mediaStartPlay();
	void showMessage(QString text);
	void showMessage(QString text, int time);
	void widgetsNeedUpdate();
    void showTime(double sec, bool flag);
    void positionChanged(int);
    void newDuration(double);
	void showFrame(int frame);
	void needResize(int w, int h);
	void noVideo();
	void volumeChanged(int);

private:
    MyProcess * proc;
    VideoWindow * m_playerWindow;
	bool we_are_restarting;
	State _state;
};

#endif
