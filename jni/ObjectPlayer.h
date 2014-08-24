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
    f32 getEnergy() const;
    f32 getMaxEnergy() const;
    scene::ICameraSceneNode* getCamera() const;

private:
    scene::ICameraSceneNode* Camera;
#ifdef DEBUG_PLAYER
    scene::ICameraSceneNode* DebugCamera;
#endif // DEBUG_PLAYER

    // accel. per one tile
    static constexpr f32 Acceleration = 0.01;
    static constexpr f32 MinSpeed = 1;
    static constexpr f32 MaxSpeed = 5;
    static constexpr f32 MaxRise = 45;
    static constexpr f32 DefaultMaxEnergy = 3;
    static constexpr f32 DefaultMaxAbsRotY = 85;
    static constexpr f32 EnergyRegenSpeed = 0.25;
    static constexpr f32 EnergyRegenCooldown = 1.5;
    static constexpr f32 AccCutoff = 3.0;
    static constexpr u32 AccSamplesSize = 3;

    f32 Height = 0.4;
    f32 Speed = MinSpeed;
    f32 Energy = DefaultMaxEnergy;
    f32 MaxEnergy = DefaultMaxEnergy;
    f32 TimeTillRegen;
    f32 FloorHeight = 0, FloorAngle = 0;
    core::vector3df TargetRot = core::vector3df(0);
    core::vector3df RotSpeed = core::vector3df(5, 5, 0);
    f32 MaxAbsRotY = DefaultMaxAbsRotY;
    bool Accelerating = false;
    bool Rising = false;

    circular_buffer<f32> AccSamples;
};

#endif // OBJECTPLAYER_H
