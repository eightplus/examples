#include "inforeader.h"

#include <QDebug>
#include <QProcess>

InfoReader * InfoReader::static_obj = 0;

InfoReader * InfoReader::obj(const QString & player_bin) {
    QString m_player_bin = player_bin;
    if (m_player_bin.isEmpty()) {
        m_player_bin = "/usr/bin/mpv";
	}
	if (!static_obj) {
        static_obj = new InfoReader(m_player_bin);
	} else {
        static_obj->setPlayerBin(m_player_bin);
	}
	return static_obj;
}

InfoReader::InfoReader(QString player_bin, QObject * parent)
	: QObject(parent)
{
    setPlayerBin(player_bin);
}

InfoReader::~InfoReader()
{
}

void InfoReader::setPlayerBin(const QString & bin)
{
    playerbin = bin;
}

void InfoReader::getInfo()
{
    vo_list.clear();
    ao_list.clear();
    demuxer_list.clear();
    vc_list.clear();
    ac_list.clear();
    vf_list.clear();

    vo_list = getList(run("--vo help"));
    ao_list = getList(run("--ao help"));
    demuxer_list = getList(run("--demuxer help"));
    vc_list = getList(run("--vd help"));
    ac_list = getList(run("--ad help"));
    {
        InfoList list = getList(run("--vf help"));
        for (int n = 0; n < list.count(); n++) {
            vf_list.append(list[n].name());
        }
    }

    option_list = getOptionsList(run("--list-options"));
}

QList<QByteArray> InfoReader::run(QString options)
{
    QList<QByteArray> r;

    QProcess proc(this);
    proc.setProcessChannelMode( QProcess::MergedChannels );

    QStringList args = options.split(" ");

    QString abs_bin;
    abs_bin = playerbin;
    proc.start(abs_bin, args);
    if (!proc.waitForStarted()) {
        qWarning("InfoReaderMPV::run: process can't start!");
        return r;
    }

    //Wait until finish
    if (!proc.waitForFinished()) {
        qWarning("InfoReaderMPV::run: process didn't finish. Killing it...");
        proc.kill();
    }

    QByteArray data = proc.readAll().replace("\r", "");
    r = data.split('\n');
    return r;
}

InfoList InfoReader::getList(const QList<QByteArray> & lines) {
    InfoList l;

    foreach(QByteArray line, lines) {
        line.replace("\n", "");
        line = line.simplified();
        if (line.startsWith("Available") || line.startsWith("demuxer:") ||
            line.startsWith("Video decoders:") || line.startsWith("Audio decoders:"))
        {
            line = QByteArray();
        }
        if (!line.isEmpty()) {
            int pos = line.indexOf(' ');
            if (pos > -1) {
                QString name = line.left(pos);
                if (name.endsWith(':')) name = name.left(name.count()-1);
                QString desc = line.mid(pos+1);
                desc = desc.replace(": ", "").replace("- ", "");
                l.append(InfoData(name, desc));
            }
        }
    }

    return l;
}

QStringList InfoReader::getOptionsList(const QList<QByteArray> & lines) {
    QStringList l;

    foreach(QByteArray line, lines) {
        line.replace("\n", "");
        line = line.simplified();
        if (line.startsWith("--")) {
            int pos = line.indexOf(' ');
            if (pos > -1) {
                QString option_name = line.left(pos);
                l << option_name;
            }
        }
    }

    return l;
}
