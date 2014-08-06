#ifndef OBJECTPLAYER_H
#define OBJECTPLAYER_H

#include "functions.h"
#include "circular_buffer.h"
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
    core::vector3df getTargetDirection() const;
    f32 getHeight() const;
    f32 getDT() const;
    scene::ICameraSceneNode* getCamera() const;

private:
    scene::ICameraSceneNode* Camera;
#ifdef DEBUG_PLAYER
    scene::ICameraSceneNode* DebugCamera;
#endif // DEBUG_PLAYER

    const f32 Acceleration = 0.1;
    const f32 MinSpeed = 1;
    const f32 MaxSpeed = 5;
    const f32 MaxRise = 20;
    const f32 DefaultMaxEnergy = 5;
    const f32 DefaultMaxAbsRotY = 40;
    const f32 AccCutoff = 3.0;
    static const u32 AccSamplesSize = 3;

    f32 Height = 0.4;
    f32 Speed = MinSpeed;
    f32 Energy = DefaultMaxEnergy;
    f32 MaxEnergy = DefaultMaxEnergy;
    f32 FloorHeight = 0, FloorAngle = 0;
    core::vector3df TargetRot = core::vector3df(0);
    core::vector3df RotSpeed = core::vector3df(5, 5, 0);
    f32 MaxAbsRotY = DefaultMaxAbsRotY;;
    bool Accelerating = false;
    bool Rising = false;

    circular_buffer<f32> AccSamples;
};

#endif // OBJECTPLAYER_H
