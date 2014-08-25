#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <irrlicht.h>
#include <irrPP.h>
#include "functions.h"

using namespace irr;

class Settings
{
public:
    Settings(SContext* cont, core::stringc storagePath);

    bool read();
    void write() const;


    f64 AccelXBias;
    video::E_POSTPROCESSING_EFFECT_QUALITY EffectQuality;
    bool Glow;
    bool Antialiasing;
    core::stringc SavegamePath;

private:
    SContext* Context;
    core::stringc FilePath;

    static constexpr u32 MagicNumber = 31415;
};

#endif // SETTINGS_H_INCLUDED
