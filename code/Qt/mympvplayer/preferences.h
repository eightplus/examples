#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

#include <QString>

class Preferences {

public:
    Preferences();
    ~Preferences();

    void reset();

	QString vo; // video output
	QString ao; // audio output

	// Audio
	bool use_soft_vol;
	int softvol_max;

	// Global volume options
	bool global_volume;
	int volume;
	bool mute;
};

#endif // _PREFERENCES_H_
