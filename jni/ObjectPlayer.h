#ifndef OBJECTPLAYER_H
#define OBJECTPLAYER_H

#include "Object.h"
#include "ObjectManager.h"
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
        f32 Speed;
        bool Accelerating;

        static const f32 Acceleration = 0.02;
        static const f32 MinSpeed = 0.02;
        static const f32 MaxSpeed = 0.08;
};

#endif // OBJECTPLAYER_H
