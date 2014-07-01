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
        scene::IQuadSceneNode* Quad;
        f32 Speed;
        f32 TargetRotY;
        f32 MaxAbsRotY;
        bool Accelerating;

        static const f32 Acceleration = 0.02;
        static const f32 MinSpeed = 0.02;
        static const f32 MaxSpeed = 0.08;
        static const f32 DefaultMaxAbsRotY = 35;
        static const f32 AccCutoff = 3.0;
        static const u32 AccSamplesSize = 10;

        f32 AccSamples[AccSamplesSize];
        u32 AccLastSampleIdx;
};

#endif // OBJECTPLAYER_H
