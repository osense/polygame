#include "Settings.h"

Settings::Settings(SContext* cont)
{
    Context = cont;

    AccelSamples = 4;
    AccelXBias = 0;
    AccelCutoff = 3.0;
    TouchController = false;
    Seed = 10;
    EffectQuality = video::EPQ_QUARTER;
    Glow = true;
}

void Settings::setStoragePath(core::stringc path)
{
    FilePath = path + "settings.json";
    SavegamePath = path + "savegame.json";
}

bool Settings::read()
{
    Json::Value root = readJson(Context, FilePath);

    if (root.get("magic_number", 0).asUInt() != MagicNumber)
        return false;

    AccelXBias = root.get("accel_x_bias", AccelXBias).asDouble();
    TouchController = root.get("touch_controller", TouchController).asBool();
    Seed = root.get("seed", 0).asUInt();
    EffectQuality = (video::E_POSTPROCESSING_EFFECT_QUALITY)root.get("effectQuality", video::EPQ_QUARTER).asUInt();
    Glow = root.get("glow", false).asBool();

    Context->Device->getLogger()->log("Loaded settings", FilePath.c_str());
    return true;
}

void Settings::write() const
{
    Json::Value root;
    root["magic_number"] = (u32) MagicNumber;
    root["seed"] = Seed;
    root["accel_x_bias"] = AccelXBias;
    root["touch_controller"] = TouchController;
    root["effectQuality"] = (u32) EffectQuality;
    root["glow"] = Glow;

    writeJson(Context, root, FilePath);

    Context->Device->getLogger()->log("Wrote settings", FilePath.c_str());
}
