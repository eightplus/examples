#ifndef _MEDIADATA_H_
#define _MEDIADATA_H_

#include <QString>
#include <QSettings>

class MediaData {

public:
	MediaData();
	virtual ~MediaData();

	virtual void reset();

    QString m_filename;
	double duration;

	int video_width;
	int video_height;

	QString stream_title;
	QString demuxer;

	QString displayName(bool show_tag = true);
};

#endif // _MEDIADATA_H_
