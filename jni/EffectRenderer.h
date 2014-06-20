#ifndef OBJECTEFFECTRENDERER_H
#define OBJECTEFFECTRENDERER_H

#include "SContext.h"
#include "ObjectVisual.h"
#include <irrlicht.h>
#include <irrPP.h>
#include "IQuadSceneNode.h"
#include "SMaterials.h"
#include "ShaderCBDoF.h"


using namespace irr;

enum E_EFFECT_TYPE
{
    EET_FXAA = 0,
    EET_DOF,
    EET_MOTION_BLUR
};


class EffectRenderer
{
public:
    EffectRenderer(SContext* cont);
    ~EffectRenderer();
    void drawAll();

    // is at least one effect active?
    bool isActive() const;

    void init(E_EFFECT_TYPE type);

private:
    static const f32 SceneQuality = 1.0;
    static const video::E_POSTPROCESSING_EFFECT_QUALITY EffectQuality = video::EPQ_QUARTER;

    SContext* Context;

    video::irrPP* PP;
    scene::ISceneManager* Smgr, *EffectSmgr;
    scene::ICameraSceneNode* Camera;
    video::ITexture* Scene, *Depth;

    video::CPostProcessingEffect* FXAA;

    video::CPostProcessingEffectChain* DoF;
    core::array<ObjectVisual*> DoFObjects;
};

#endif // OBJECTEFFECTRENDERER_H
