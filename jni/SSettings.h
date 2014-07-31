#ifndef SSETTINGS_H_INCLUDED
#define SSETTINGS_H_INCLUDED

#define MAGIC_NUMBER_KOKOT 7579757984

#include <irrTypes.h>

using namespace irr;

struct SSettings
{
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
