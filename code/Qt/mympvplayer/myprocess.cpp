#include "myprocess.h"
#include "inforeader.h"

#include <QProcess>
#include <QDebug>

MyProcess::MyProcess(QObject * parent) : QProcess(parent)
  , notified_player_is_running(false)
  , notified_pause(false)
  , received_end_of_file(false)
{
	clearArguments();
	setProcessChannelMode( QProcess::MergedChannels );
	
    connect(this, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOut()));
    connect(this, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(procFinished()));
    connect(this, SIGNAL(lineAvailable(QByteArray)), this, SLOT(parseLine(QByteArray)));
    connect(this, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int,QProcess::ExitStatus)));

    initializeRX();
}

void MyProcess::clearArguments()
{
	program = "";
	arg.clear();
}

bool MyProcess::isRunning()
{
	return (state() == QProcess::Running);
}

bool MyProcess::start()
{
    md.reset();
    notified_player_is_running = false;
    notified_pause = false;
    received_end_of_file = false;
	remaining_output.clear();
    QProcess::start(program, arg);

    return waitForStarted();
}

void MyProcess::readStdOut()
{
    genericRead(readAllStandardOutput());
}

void MyProcess::genericRead(QByteArray buffer)
{
	QByteArray ba = remaining_output + buffer;
	int start = 0;
	int from_pos = 0;
	int pos = canReadLine(ba, from_pos);
    while (pos > -1) {
		QByteArray line = ba.mid(start, pos-start);
		from_pos = pos + 1;
		start = from_pos;
		emit lineAvailable(line);//准备解析line
		pos = canReadLine(ba, from_pos);
	}

	remaining_output = ba.mid(from_pos);
}

int MyProcess::canReadLine(const QByteArray & ba, int from) {
	int pos1 = ba.indexOf('\n', from);
	int pos2 = ba.indexOf('\r', from);

	if ( (pos1 == -1) && (pos2 == -1) ) return -1;

	int pos = pos1;
	if ( (pos1 != -1) && (pos2 != -1) ) {
		if (pos1 < pos2) pos = pos1; else pos = pos2;
	} else {
		if (pos1 == -1) pos = pos2;
		else
		if (pos2 == -1) pos = pos1;
	}

	return pos;
}

void MyProcess::procFinished()
{
    if (bytesAvailable() > 0) readStdOut();
}

void MyProcess::writeToStdin(QString text)
{
    if (isRunning()) {
        write( text.toLocal8Bit() + "\n");
    } else {
        qWarning("PlayerProcess::writeToStdin: process not running");
    }
}

void MyProcess::initializeRX()
{
    rx_av.setPattern("^STATUS: ([0-9\\.-]+) / ([0-9\\.-]+) P: (yes|no) B: (yes|no) I: (yes|no) VB: ([0-9\\.-]+) AB: ([0-9\\.-]+)");
    rx_dsize.setPattern("^INFO_VIDEO_DSIZE=(\\d+)x(\\d+)");
    rx_vo.setPattern("^VO: \\[(.*)\\]");
    rx_ao.setPattern("^AO: \\[(.*)\\]");
    rx_paused.setPattern("^\\(Paused\\)");
    rx_endoffile.setPattern("^Exiting... \\(End of file\\)");
    rx_audio.setPattern("^.* Audio\\s+--aid=(\\d+)( --alang=([a-zA-Z-]+)|)([ \\(\\)\\*]+)('(.*)'|)");
    rx_videocodec.setPattern("^INFO_VIDEO_CODEC=(.*)\\s");
    rx_videocodec.setMinimal(true);
    rx_audiocodec.setPattern("^INFO_AUDIO_CODEC=(.*)\\s");
    rx_audiocodec.setMinimal(true);
    rx_forbidden.setPattern("HTTP error 403 Forbidden");
    rx_playing.setPattern("^Playing:.*|^\\[ytdl_hook\\].*");
    rx_generic.setPattern("^([A-Z_]+)=(.*)");
    rx_stream_title.setPattern("icy-title: (.*)");
    rx_debug.setPattern("^(INFO|METADATA)_.*=\\$.*");
}

void MyProcess::parseLine(QByteArray ba)
{
    if (ba.isEmpty()) return;
    QString tag;
    QString value;
    QString line = QString::fromLocal8Bit(ba);
    static double last_sec = -1;

    // Parse A: V: line
    if (rx_av.indexIn(line) > -1) {
        double sec = rx_av.cap(1).toDouble();
        double length = rx_av.cap(2).toDouble();
        bool paused = (rx_av.cap(3) == "yes");
        bool buffering = (rx_av.cap(4) == "yes");
        bool idle = (rx_av.cap(5) == "yes");
        if (length != md.duration) {
            md.duration = length;
        }

        if (paused && notified_pause) {
            if (last_sec != sec) {
                last_sec = sec;
                emit receivedCurrentSec(sec);
                emit receivedPause();
            }
            return;
        }

        if (paused) {
            notified_pause = true;
            emit receivedPause();
            return;
        }
        else
        if (buffering) {
            emit receivedBuffering();
            return;
        }
        else
        if (idle) {
            emit receivedBuffering();
            return;
        }
        notified_pause = false;

        if (!notified_player_is_running) {
            if (md.video_width == 0 || md.video_height == 0) {
                emit receivedNoVideo();
            }

            emit playerFullyLoaded();
            emit receivedCurrentFrame(0); // Ugly hack: set the frame counter to 0
            notified_player_is_running = true;
        }

        emit receivedCurrentSec( sec );
    }
    else {
        emit lineAvailable(line);
        // End of file
        if (rx_endoffile.indexIn(line) > -1)  {
            qDebug("MVPProcess::parseLine: detected end of file");
            if (!received_end_of_file) {
                // In case of playing VCDs or DVDs, maybe the first title
                // is not playable, so the GUI doesn't get the info about
                // available titles. So if we received the end of file
                // first let's pretend the file has started so the GUI can have
                // the data.
                if ( !notified_player_is_running) {
                    emit playerFullyLoaded();
                }
                // Send signal once the process is finished, not now!
                received_end_of_file = true;
            }
        }
        else
        // Window resolution
        if (rx_dsize.indexIn(line) > -1) {
            int w = rx_dsize.cap(1).toInt();
            int h = rx_dsize.cap(2).toInt();
            emit receivedWindowResolution( w, h );
        }
        else
        // VO
        if (rx_vo.indexIn(line) > -1) {
            emit receivedVO( rx_vo.cap(1) );
            // Ask for window resolution
            writeToStdin("print_text INFO_VIDEO_DSIZE=${=dwidth}x${=dheight}");
        }
        else
        // AO
        if (rx_ao.indexIn(line) > -1) {
            emit receivedAO( rx_ao.cap(1) );
        }
        else
        //Playing
        if (rx_playing.indexIn(line) > -1) {
            emit receivedPlaying();
        }
        else
        if (rx_forbidden.indexIn(line) > -1) {
            qDebug("MVPProcess::parseLine: 403 forbidden");
            emit receivedForbiddenText();
        }

        //Generic things
        if (rx_generic.indexIn(line) > -1) {
            tag = rx_generic.cap(1);
            value = rx_generic.cap(2);

            if (tag == "INFO_VIDEO_WIDTH") {
                md.video_width = value.toInt();
                //qDebug("MPVProcess::parseLine: md.video_width set to %d", md.video_width);
            }
            else
            if (tag == "INFO_VIDEO_HEIGHT") {
                md.video_height = value.toInt();
                //qDebug("MPVProcess::parseLine: md.video_height set to %d", md.video_height);
            }
        }
    }
}

// Called when the process is finished
void MyProcess::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug("MPVProcess::processFinished: exitCode: %d, status: %d", exitCode, (int) exitStatus);
    // Send this signal before the endoffile one, otherwise
    // the playlist will start to play next file before all
    // objects are notified that the process has exited.
    emit processExited();
    if (received_end_of_file) emit receivedEndOfFile();
}

void MyProcess::setMedia(const QString & media, bool is_playlist) {
    arg << "--term-playing-msg="
            "MPV_VERSION=${=mpv-version:}\n"
            "INFO_VIDEO_WIDTH=${=width}\nINFO_VIDEO_HEIGHT=${=height}\n"
            "INFO_VIDEO_ASPECT=${=video-aspect}\n"
//			"INFO_VIDEO_DSIZE=${=dwidth}x${=dheight}\n"
            "INFO_VIDEO_FPS=${=container-fps:${=fps}}\n"
//			"INFO_VIDEO_BITRATE=${=video-bitrate}\n"
            "INFO_VIDEO_FORMAT=${=video-format}\n"
            "INFO_VIDEO_CODEC=${=video-codec}\n"

//			"INFO_AUDIO_BITRATE=${=audio-bitrate}\n"
//			"INFO_AUDIO_FORMAT=${=audio-format}\n" // old
            "INFO_AUDIO_FORMAT=${=audio-codec-name}\n"
            "INFO_AUDIO_CODEC=${=audio-codec}\n"
//			"INFO_AUDIO_RATE=${=audio-samplerate}\n" // old
            "INFO_AUDIO_RATE=${=audio-params/samplerate}\n"
//			"INFO_AUDIO_NCH=${=audio-channels}\n" // old
            "INFO_AUDIO_NCH=${=audio-params/channel-count}\n"

//			"INFO_LENGTH=${=length}\n"
            "INFO_LENGTH=${=duration:${=length}}\n"

            "INFO_DEMUXER=${=current-demuxer:${=demuxer}}\n"
            "INFO_SEEKABLE=${=seekable}\n"
            "INFO_TITLES=${=disc-titles}\n"
            "INFO_CHAPTERS=${=chapters}\n"
            "INFO_TRACKS_COUNT=${=track-list/count}\n"

            "METADATA_TITLE=${metadata/by-key/title:}\n"
            "METADATA_ARTIST=${metadata/by-key/artist:}\n"
            "METADATA_ALBUM=${metadata/by-key/album:}\n"
            "METADATA_GENRE=${metadata/by-key/genre:}\n"
            "METADATA_DATE=${metadata/by-key/date:}\n"
            "METADATA_TRACK=${metadata/by-key/track:}\n"
            "METADATA_COPYRIGHT=${metadata/by-key/copyright:}\n"

            "INFO_MEDIA_TITLE=${=media-title:}\n"
            "INFO_STREAM_PATH=${stream-path}\n";

    arg << "--audio-client-name=KylinVideo";
    arg << "--term-status-msg=STATUS: ${=time-pos} / ${=duration:${=length:0}} P: ${=pause} B: ${=paused-for-cache} I: ${=core-idle} VB: ${=video-bitrate:0} AB: ${=audio-bitrate:0}";

    if (is_playlist) {
        arg << "--playlist=" + media;
    } else {
        arg << media;
    }
}

void MyProcess::setFixedOptions()
{
    arg << "--no-config";
    arg << "--no-quiet";
    arg << "--terminal";
    arg << "--no-msg-color";
    arg << "--input-file=/dev/stdin";

    static QStringList option_list;
    InfoReader * ir = InfoReader::obj(executable());
    ir->getInfo();
    option_list = ir->optionList();
    if (option_list.contains("--gpu-context")) {
        arg << "--gpu-context=x11egl";
    }
}

void MyProcess::disableInput()
{
    arg << "--no-input-default-bindings";
    if (isOptionAvailable("--input-vo-keyboard")) {
        arg << "--input-vo-keyboard=no";
    } else {
        arg << "--input-x11-keyboard=no";
    }
    arg << "--no-input-cursor";
    arg << "--cursor-autohide=no";
}

bool MyProcess::isOptionAvailable(const QString & option)
{
    static QStringList option_list;
    static QString mpv_bin;

    if (option_list.isEmpty() || mpv_bin != executable()) {
        InfoReader * ir = InfoReader::obj(executable());
        ir->getInfo();
        option_list = ir->optionList();
        mpv_bin = executable();
        //qDebug() << "MPVProcess::isOptionAvailable: option_list:" << option_list;
    }

    return option_list.contains(option);
}

void MyProcess::setOption(const QString & option_name, const QVariant & value)
{
    if (option_name == "cache") {
        int cache = value.toInt();
        if (cache > 31) {
            arg << "--cache=" + value.toString();
        } else {
            arg << "--cache=no";
        }
    }
    else
    if (option_name == "cache_auto") {
        arg << "--cache=auto";
    }
    else
    if (option_name == "ss") {
        arg << "--start=" + value.toString();
    }
    else
    if (option_name == "endpos") {
        arg << "--length=" + value.toString();
    }
    else
    if (option_name == "loop") {
        QString loop = "--loop";
        if (isOptionAvailable("--loop-file")) loop = "--loop-file";
        QString o = value.toString();
        if (o == "0") o = "inf";
        arg << loop + "=" + o;
    }
    else
    if (option_name == "ass") {
        arg << "--sub-ass";
    }
    else
    if (option_name == "noass") {
        arg << "--no-sub-ass";
    }
    else
    if (option_name == "sub-fuzziness") {
        QString v;
        switch (value.toInt()) {
            case 1: v = "fuzzy"; break;
            case 2: v = "all"; break;
            default: v = "exact";
        }
        arg << "--sub-auto=" + v;
    }
    else
    if (option_name == "audiofile") {
        arg << "--audio-file=" + value.toString();
    }
    else
    if (option_name == "delay") {
        arg << "--audio-delay=" + value.toString();
    }
    else
    if (option_name == "subdelay") {
        arg << "--sub-delay=" + value.toString();
    }
    else
    if (option_name == "sub") {
        arg << "--sub-file=" + value.toString();
    }
    else
    if (option_name == "subpos") {
        arg << "--sub-pos=" + value.toString();
    }
    else
    if (option_name == "font") {
        arg << "--osd-font=" + value.toString();
    }
    else
    if (option_name == "subcp") {
        QString cp = value.toString();
        if (!cp.startsWith("enca")) cp = "utf8:" + cp;
        arg << "--sub-codepage=" + cp;
    }
    else
    if (option_name == "osd-level") {
        arg << "--osd-level=" + value.toString();
    }
    else
    if (option_name == "osd-fractions") {
        bool use_fractions = value.toBool();
        if (use_fractions) arg << "--osd-fractions";
    }
    else
    if (option_name == "osd-bar-pos") {
        if (isOptionAvailable("--osd-bar-align-y")) {
            if (value.toInt() >= 0 && value.toInt() <= 100) {
                double position = double (value.toInt() * 2 - 100) / 100;
                arg << "--osd-bar-align-y=" + QString::number(position);
            }
        }
    }
    else
    if (option_name == "sws") {
        arg << "--sws-scaler=lanczos";
    }
    else
    if (option_name == "channels") {
        arg << "--audio-channels=" + value.toString();
    }
    else
    if (option_name == "subfont-text-scale" || option_name == "ass-font-scale") {
        arg << "--sub-scale=" + value.toString();
    }
    else
    if (option_name == "ass-line-spacing") {
        QString line_spacing = "--ass-line-spacing";
        if (isOptionAvailable("--sub-ass-line-spacing")) line_spacing = "--sub-ass-line-spacing";
        arg << line_spacing + "=" + value.toString();
    }
    else
    if (option_name == "ass-force-style") {
        QString ass_force_style = "--ass-force-style";
        if (isOptionAvailable("--sub-ass-force-style")) ass_force_style = "--sub-ass-force-style";
        arg << ass_force_style + "=" + value.toString();
    }
    else
    if (option_name == "stop-xscreensaver") {
        bool stop_ss = value.toBool();
        if (stop_ss) arg << "--stop-screensaver"; else arg << "--no-stop-screensaver";
    }
    else
    if (option_name == "correct-pts") {
        bool b = value.toBool();
        if (b) arg << "--correct-pts"; else arg << "--no-correct-pts";
    }
    else
    if (option_name == "idx") {
        arg << "--index=default";
    }
    else
    if (option_name == "softvol") {
        if (value.toString() == "off") {
            if (isOptionAvailable("--volume-max")) {
                arg << "--volume-max=100";
            }
        } else {
            int v = value.toInt();
            if (v < 100) v = 100;
            if (isOptionAvailable("--volume-max")) {
                arg << "--volume-max=" + QString::number(v);
            } else {
                arg << "--softvol=yes";
                arg << "--softvol-max=" + QString::number(v);
            }
        }
    }
    else
    if (option_name == "subfps") {
        arg << "--sub-fps=" + value.toString();
    }
    else
    if (option_name == "forcedsubsonly") {
        arg << "--sub-forced-only";
    }
    else
    if (option_name == "prefer-ipv4" || option_name == "prefer-ipv6" ||
        option_name == "dr" || option_name == "double" ||
        option_name == "adapter" || option_name == "edl" ||
        option_name == "slices" || option_name == "colorkey" ||
        option_name == "subcc" || option_name == "vobsub" ||
        option_name == "zoom" || option_name == "flip-hebrew" ||
        option_name == "autoq")
    {
        // Ignore
    }
    else
    if (option_name == "tsprog") {
        // Unsupported
    }
    else
    if (option_name == "threads") {
        arg << "--vd-lavc-threads=" + value.toString();
    }
    else
    if (option_name == "skiploopfilter") {
        arg << "--vd-lavc-skiploopfilter=all";
    }
    else
    if (option_name == "keepaspect" || option_name == "fs") {
        bool b = value.toBool();
        if (b) arg << "--" + option_name; else arg << "--no-" + option_name;
    }
    else
    if (option_name == "vo") {
        QString vo = value.toString();
        if (vo.endsWith(",")) vo.chop(1);
        if (isOptionAvailable("--xv-adaptor")) {
            QRegExp rx("xv:adaptor=(\\d+)");
            if (rx.indexIn(vo) > -1) {
                QString adaptor = rx.cap(1);
                vo = "xv";
                arg << "--xv-adaptor=" + adaptor;
            }
        }
        int pos = vo.indexOf(":");
        if (pos > -1) vo = vo.left(pos);

        if (vo == "gl") vo = "opengl";

        arg << "--vo=" + vo + ",";
    }
    else
    if (option_name == "ao") {
        QString ao = value.toString();

        QStringList l;
        if (ao.contains(":")) l = ao.split(":::");;
        if (l.count() > 0) ao = l[0];

        if (isOptionAvailable("--audio-device")) {
            if (l.count() == 3) {
                if (l[0] == "pulse") {
                    arg << "--audio-device=pulse/" + l[2];
                }
            }
        } else {
            if (l.count() > 1) {
                if (l[0] == "alsa") {
                    ao = "alsa:device=[hw:" + l[1] + "]";
                }
                else
                if (l[0] == "pulse") {
                    ao = "pulse::" + l[1];
                }
            }
        }

        // Remove options (used by player)
        int pos = ao.indexOf(":");
        if (pos > -1) ao = ao.left(pos);

        arg << "--ao=" + ao + ",";
    }
    else
    if (option_name == "vc") {
        qDebug() << "MPVProcess::setOption: video codec ignored";
    }
    else
    if (option_name == "ac") {
        qDebug() << "MPVProcess::setOption: audio codec ignored";
    }
    else
    if (option_name == "afm") {
        QString s = value.toString();
        if (s == "hwac3") arg << "--audio-spdif=ac3,dts-hd,truehd";
    }
    else
    if (option_name == "enable_streaming_sites_support") {
        if (isOptionAvailable("--ytdl")) {
            if (value.toBool()) arg << "--ytdl"; else arg << "--ytdl=no";
        }
    }
    else
    if (option_name == "ytdl_quality") {
        if (isOptionAvailable("--ytdl-format")) {
            QString quality = value.toString();
            if (!quality.isEmpty()) arg << "-ytdl-format=" + quality;
        }
    }
    else
    if (option_name == "fontconfig") {
        if (isOptionAvailable("--use-text-osd")) {
            bool b = value.toBool();
            if (b) arg << "--use-text-osd=yes"; else arg << "--use-text-osd=no";
        }
    }
    else
    if (option_name == "mute") {
        arg << "--mute=yes";
    }
    else
    if (option_name == "scaletempo") {
        if (isOptionAvailable("--audio-pitch-correction")) {
            bool enabled = value.toBool();
            if (enabled) arg << "--audio-pitch-correction=yes"; else arg << "--audio-pitch-correction=no";
        }
    }
    else
    if (option_name == "vf-add") {
        if (!value.isNull()) arg << "--vf-add=" + value.toString();
    }
    else
    if (option_name == "af-add") {
        if (!value.isNull()) arg << "--af-add=" + value.toString();
    }
    else
    if (option_name == "aid" || option_name == "sid" || option_name == "secondary-sid" || option_name == "vid") {
        int v = value.toInt();
        arg << QString("--%1=%2").arg(option_name).arg(v > -1 ? value.toString() : "no");
    }
    else
    if (option_name == "wid" ||
        option_name == "alang" || option_name == "slang" ||
        option_name == "volume" ||
        option_name == "ass-styles" ||
        option_name == "embeddedfonts" ||
        option_name == "osd-scale" ||
        option_name == "speed" ||
        option_name == "contrast" || option_name == "brightness" ||
        option_name == "hue" || option_name == "saturation" || option_name == "gamma" ||
        option_name == "monitorpixelaspect" || option_name == "monitoraspect" ||
        option_name == "mc" ||
        option_name == "framedrop" ||
        option_name == "priority" ||
        option_name == "hwdec" ||
        option_name == "autosync" ||
        option_name == "dvd-device" || option_name == "cdrom-device" ||
        option_name == "demuxer" ||
        option_name == "frames" ||
        option_name == "user-agent" || option_name == "referrer" ||
        option_name == "ab-loop-a" || option_name == "ab-loop-b")
    {
        QString s = "--" + option_name;
        if (!value.isNull()) s += "=" + value.toString();
        arg << s;
    }
    else
    {
        qDebug() << "MPVProcess::setOption: unknown option:" << option_name;
    }
}

void MyProcess::quit()
{
    writeToStdin("quit 0");
}

void MyProcess::setVolume(int v)
{
    writeToStdin("set volume " + QString::number(v));
}

void MyProcess::seek(double secs, int mode, bool precise)
{
    QString s = "seek " + QString::number(secs) + " ";
    switch (mode) {
        case 0 : s += "relative "; break;
        case 1 : s += "absolute-percent "; break;
        case 2 : s += "absolute "; break;
    }
    if (precise) s += "exact"; else s += "keyframes";
    writeToStdin(s);
}

void MyProcess::mute(bool b)
{
    writeToStdin(QString("set mute %1").arg(b ? "yes" : "no"));
}

void MyProcess::setPause(bool b)
{
    writeToStdin(QString("set pause %1").arg(b ? "yes" : "no"));
}
