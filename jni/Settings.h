#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

#include <irrlicht.h>
#include <irrPP.h>
#include "functions.h"

using namespace irr;

class Settings
{
public:
    Settings(SContext* cont);
    
    void setStoragePath(core::stringc path);

    bool read();
    void write() const;


    u32 AccelSamples;
    f64 AccelXBias;
    f32 AccelCutoff;
    bool TouchController;
    u32 Seed;
    video::E_POSTPROCESSING_EFFECT_QUALITY EffectQuality;
    bool Glow;
    core::stringc SavegamePath;

private:
    SContext* Context;
    core::stringc FilePath;

    static constexpr u32 MagicNumber = 31415;
};

#endif // SETTINGS_H_INCLUDED
