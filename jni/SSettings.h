#ifndef SSETTINGS_H_INCLUDED
#define SSETTINGS_H_INCLUDED

#define SETTINGS_FILENAME "settings.json"
#define MAGIC_NUMBER 9001

#include <irrlicht.h>
#include <irrPP.h>

using namespace irr;

struct SSettings
{
    core::stringc FilePath;

    video::E_POSTPROCESSING_EFFECT_QUALITY EffectQuality;
    bool Glow;
    bool Antialiasing;
};

#endif // SSETTINGS_H_INCLUDED
