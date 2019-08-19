#ifndef INFOREADER_H
#define INFOREADER_H

#include <QObject>
#include <QList>
#include <QStringList>

class InfoData {

public:
	InfoData() {};
	InfoData( QString name, QString desc) {
		_name = name;
		_desc = desc;
	};
	~InfoData() {};

	void setName(QString name) { _name = name; };
	void setDesc(QString desc) { _desc = desc; };

	QString name() const { return _name; };
	QString desc() const { return _desc; };

	bool operator<(const InfoData & other) const {
		return name() < other.name();
	}

	bool operator==(const InfoData & other) const {
		return name() == other.name();
	}

private:
	QString _name, _desc;
};


typedef QList<InfoData> InfoList;


class InfoReader : QObject {
	Q_OBJECT

public:
    InfoReader(QString player_bin, QObject * parent = 0);
	~InfoReader();

    void setPlayerBin(const QString & bin);

    void getInfo();

	InfoList voList() { return vo_list; };
	InfoList aoList() { return ao_list; };

    InfoList demuxerList() { return demuxer_list; };
    InfoList vcList() { return vc_list; };
    InfoList acList() { return ac_list; };

	QStringList vfList() { return vf_list; };
	QStringList optionList() { return option_list; };

    static InfoReader * obj(const QString & player_bin = QString::null);

private:
	static InfoReader * static_obj;

protected:
    QList<QByteArray> run(QString options);
    InfoList getList(const QList<QByteArray> &);
    QStringList getOptionsList(const QList<QByteArray> &);

protected:
    QString playerbin;

    InfoList vo_list;
    InfoList ao_list;

    InfoList demuxer_list;
    InfoList vc_list;
    InfoList ac_list;

    QStringList vf_list;
    QStringList option_list;
};

#endif // INFOREADER_H
