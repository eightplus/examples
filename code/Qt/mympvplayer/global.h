
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <QString>

class Preferences;

namespace Global {
	extern Preferences * pref;
    void global_init();
	void global_end();
}

#endif

