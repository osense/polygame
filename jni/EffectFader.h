#ifndef OBJECTFADER_H
#define OBJECTFADER_H

#include "SContext.h"
#include "SMaterials.h"
#include "ShaderCBFader.h"
#include <irrlicht.h>

using namespace irr;

class EffectFader
{
public:
    EffectFader(SContext* cont);
    ~EffectFader();

    void init();

    void startFadeOut(f32 strength, f32 fadeTime = 1, f32 strengthStart = 0);
    void startFadeInContinuous();
    void startFadeIn(f32 strengthStart = 1, f32 fadeTime = 1);

    void draw(f32 delta);
    bool isActive() const;

    void setIncludeGUI(bool incl);
    bool getIncludeGUI() const;

private:
    scene::SMesh* createQuadMesh();

    SContext* Context;

    enum E_FADE_STATE
    {
        EFS_NONE,
        EFS_FADEOUT,
        EFS_FADED_OUT,
        EFS_FADEIN
    } State;

    bool IncludeGUI;
    f32 Strength = 0, TargetStrength = 1, StartStrength = 0;
    f32 FadeTime = 0;

    scene::SMeshBuffer* MeshBuffer = 0;
};

#endif // OBJECTFADER_H
