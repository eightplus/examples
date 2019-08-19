#include "preferences.h"
#include "global.h"

using namespace Global;

Preferences::Preferences()
{
	reset();
}

Preferences::~Preferences()
{

}

void Preferences::reset()
{
    vo = "xv";
    ao = "pulse";
	use_soft_vol = true;
    softvol_max = 110;
	global_volume = true;
	volume = 50;
	mute = false;
}
