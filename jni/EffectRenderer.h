#ifndef OBJECTEFFECTRENDERER_H
#define OBJECTEFFECTRENDERER_H

#include "SContext.h"
#include <irrlicht.h>
#include <irrPP.h>
#include "EffectFader.h"
#include "SMaterials.h"
#include "Settings.h"


using namespace irr;

enum E_EFFECT_TYPE
{
    EET_GLOW,
    EET_FADER
};


class EffectRenderer
{
public:
    EffectRenderer(SContext* cont);
    ~EffectRenderer();
    void drawAll(u32 timeDelta);

    // is at least one effect active?
    bool isActive() const;

    void init(E_EFFECT_TYPE type);

    void loadPP(bool reload = false);

    EffectFader* getFader() const;

    video::irrPP* PP;

private:
    SContext* Context;
    bool Active;

    scene::ISceneManager* Smgr;
    video::IVideoDriver* Video;
    video::ITexture* Scene;

    video::CPostProcessingEffectChain* Glow;
    EffectFader* Fader;
};

#endif // OBJECTEFFECTRENDERER_H
