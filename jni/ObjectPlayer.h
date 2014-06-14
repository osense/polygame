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

    private:
        scene::ICameraSceneNode* Camera;
};

#endif // OBJECTPLAYER_H
