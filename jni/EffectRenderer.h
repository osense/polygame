#ifndef OBJECTEFFECTRENDERER_H
#define OBJECTEFFECTRENDERER_H

#include "SContext.h"
#include "ObjectVisual.h"
#include <irrlicht.h>
#include "IQuadSceneNode.h"
#include "SMaterials.h"
#include "ShaderCBDoF.h"


using namespace irr;


class EffectRenderer
{
public:
    EffectRenderer(SContext* cont);
    ~EffectRenderer();
    void drawAll();

    // is at least one effect active?
    bool isActive() const;

    void setDoFEnabled(bool enable);

private:
    static const f32 SceneQuality = 1.0;
    static const f32 EffectQuality = 0.5;

    enum E_EFFECT_TYPE
    {
        EET_DOF = 0
    };

    void initRT(E_EFFECT_TYPE type);

    SContext* Context;

    scene::ISceneManager* Smgr, *EffectSmgr;
    scene::ICameraSceneNode* Camera;
    scene::IQuadSceneNode* Quad;
    video::ITexture* Scene, *Depth;

    bool DoFEnabled;
    core::array<ObjectVisual*> DoFObjects;
    video::E_MATERIAL_TYPE DoFMaterial;
};

#endif // OBJECTEFFECTRENDERER_H
