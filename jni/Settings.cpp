#include "Settings.h"

Settings::Settings(SContext* cont, core::stringc storagePath)
{
    Context = cont;
    FilePath = storagePath + "settings.json";
    SavegamePath = storagePath + "savegame.json";

    EffectQuality = video::EPQ_QUARTER;
    Glow = true;
    Antialiasing = false;
}

bool Settings::read()
{
    Json::Value root = readJson(Context, FilePath);

    if (root.get("magic_number", 0).asUInt() != MagicNumber)
        return false;

    EffectQuality = (video::E_POSTPROCESSING_EFFECT_QUALITY)root.get("effectQuality", video::EPQ_QUARTER).asUInt();
    Glow = root.get("glow", false).asBool();
    Antialiasing = root.get("antialiasing", false).asBool();

    Context->Device->getLogger()->log("Loaded settings", FilePath.c_str());
    return true;
}

void Settings::write() const
{
    Json::Value root;
    root["magic_number"] = (u32) MagicNumber;
    root["effectQuality"] = (u32) EffectQuality;
    root["glow"] = Glow;
    root["antialiasing"] = Antialiasing;

    writeJson(Context, root, FilePath);

    Context->Device->getLogger()->log("Wrote settings", FilePath.c_str());
}
