#ifndef MEDIASETTINGS_H
#define MEDIASETTINGS_H

#include <QString>

class MediaSettings {
public:
	MediaSettings();
    ~MediaSettings();

    void reset();

    double current_sec;
	int volume;
	bool mute;
    double zoom_factor;
    bool loop;
	QString current_demuxer;
	QString forced_demuxer;
	int win_width;
	int win_height;
};

#endif // MEDIASETTINGS_H
