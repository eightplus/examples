#include "global.h"
#include "preferences.h"

#include <QApplication>
#include <QDebug>

Preferences * Global::pref = 0;

using namespace Global;

void Global::global_init()
{
    pref = new Preferences();
}

void Global::global_end()
{
	delete pref;
	pref = 0;
}
