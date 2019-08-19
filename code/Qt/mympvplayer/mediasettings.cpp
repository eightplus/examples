#include "mediasettings.h"
#include "preferences.h"
#include "global.h"

using namespace Global;

MediaSettings::MediaSettings() {
	reset();
}

MediaSettings::~MediaSettings() {
}

void MediaSettings::reset()
{
    current_sec = 0;
    volume = 50;
	mute = false;
	loop = false;
	current_demuxer = "unknown";
	forced_demuxer="";
	win_width=400;
	win_height=300;
}
