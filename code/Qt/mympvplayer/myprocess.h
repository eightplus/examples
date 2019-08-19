#ifndef _MY_PROCESS_H_
#define _MY_PROCESS_H_

#include <QProcess>
#include <QTemporaryFile>
#include <QTimer>
#include <QVariant>
#include <QDebug>
#include <QString>
#include <QRegExp>

#include "mediadata.h"

class MyProcess : public QProcess
{
	Q_OBJECT

public:
	MyProcess ( QObject * parent = 0 );

    void writeToStdin(QString text);
    MediaData mediaData() { return md; };

	virtual void setExecutable(const QString & p) { program = p; };
	QString executable() { return program; };

    void clearArguments();
    bool start();
    bool isRunning();
    void setMedia(const QString & media, bool is_playlist = false);
    void disableInput();
    void setFixedOptions();
    void setOption(const QString & option_name, const QVariant & value = QVariant());
    void quit();
    void setVolume(int v);
    void seek(double secs, int mode, bool precise);
    void mute(bool b);
    void setPause(bool b);

    MediaData md;

protected:
    bool isOptionAvailable(const QString & option);
    int canReadLine(const QByteArray & ba, int from = 0);
    void genericRead(QByteArray buffer);

    QString program;
    QStringList arg;
    QByteArray remaining_output;

protected slots:
    void readStdOut();
    void procFinished();
    void parseLine(QByteArray ba);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void processExited();
    void lineAvailable(QByteArray ba);
    void lineAvailable(QString line);
    void receivedCurrentSec(double sec);
    void receivedCurrentFrame(int frame);
    void receivedPause();
    void receivedWindowResolution(int,int);
    void receivedNoVideo();
    void receivedVO(QString);
    void receivedAO(QString);
    void receivedEndOfFile();
    void playerFullyLoaded();
    void receivedCacheMessage(QString);
    void receivedCacheEmptyMessage(QString);
    void receivedCreatingIndex(QString);
    void receivedConnectingToMessage(QString);
    void receivedResolvingMessage(QString);
    void receivedBuffering();
    void receivedPlaying();
    void receivedScreenshot(QString);
    void receivedScanningFont(QString);
    void receivedForbiddenText();
    void receivedStreamTitle(QString);
    void receivedStreamTitleAndUrl(QString,QString);

private:
    bool notified_player_is_running;
    bool notified_pause;
    bool received_end_of_file;

    QRegExp rx_av;
    QRegExp rx_dsize;
    QRegExp rx_vo;
    QRegExp rx_ao;
    QRegExp rx_paused;
    QRegExp rx_endoffile;
    QRegExp rx_audio;
    QRegExp rx_videocodec;
    QRegExp rx_audiocodec;
    QRegExp rx_forbidden;
    QRegExp rx_playing;
    QRegExp rx_generic;
    QRegExp rx_stream_title;
    QRegExp rx_debug;

    void initializeRX();
};

#endif // _MY_PROCESS_H_
