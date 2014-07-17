#ifndef OBJECTPLAYER_H
#define OBJECTPLAYER_H

#include "functions.h"
#include "Object.h"
#include "ObjectManager.h"
#include "EffectRenderer.h"
#include <irrlicht.h>

using namespace irr;

class ObjectPlayer : public Object
{
public:
    ObjectPlayer(SContext* cont);
    virtual ~ObjectPlayer();
    virtual void onMessage(SMessage msg);

    f32 getSpeed() const;
    core::vector3df getDirection() const;

private:
    scene::ICameraSceneNode* Camera;
#ifdef DEBUG_PLAYER
    scene::ICameraSceneNode* DebugCamera;
#endif // DEBUG_PLAYER

    //scene::IQuadSceneNode* Quad;
    f32 Speed;
    f32 TargetRotY;
    f32 MaxAbsRotY;
    bool Accelerating;

    static const f32 Acceleration = 1;
    static const f32 MinSpeed = 1;
    static const f32 MaxSpeed = 5;
    static const f32 DefaultMaxAbsRotY = 35;
    static const f32 AccCutoff = 3.0;
    static const u32 AccSamplesSize = 5;

    f32 AccSamples[AccSamplesSize];
    u32 AccLastSampleIdx;
};

#endif // OBJECTPLAYER_H
