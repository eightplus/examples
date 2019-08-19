#include "mediadata.h"

#include <QFileInfo>
#include <QDebug>

MediaData::MediaData()
{
	reset();
}

MediaData::~MediaData()
{
}

void MediaData::reset()
{
    m_filename="";
	duration=0;
	video_width=0;
    video_height=0;
	stream_title = "";
	demuxer="";
}

QString MediaData::displayName(bool show_tag)
{
	if (show_tag) {
        if (!stream_title.isEmpty()) {
            return stream_title;
        }
	}


    QString name;
    if (name.isEmpty()) {
        QFileInfo fi(m_filename);
        if (fi.exists()) {
            // Local file
            name = fi.fileName();
        } else {
            // Stream
            name = m_filename;
        }
    }

    return name;
}
