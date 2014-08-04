#ifndef SSETTINGS_H_INCLUDED
#define SSETTINGS_H_INCLUDED

#define SETTINGS_FILENAME "settings.json"
#define MAGIC_NUMBER 9001

#include <irrlicht.h>

using namespace irr;

struct SSettings
{
    core::stringc FilePath;

    enum E_GLOW_SETTING
    {
        EGS_OFF = 0,
        EGS_LOW = 1,
        EGS_MEDIUM = 2,
        EGS_HIGH = 3
    } Glow;

    bool Antialiasing;
};

#endif // SSETTINGS_H_INCLUDED
