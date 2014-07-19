#ifndef OBJECTDEBUGINFO_H
#define OBJECTDEBUGINFO_H

//#ifdef DEBUG_INFO

#include "Object.h"
#include "ObjectManager.h"
#include <irrlicht.h>

#include "ObjectEventReceiver.h"
#include "ObjectPlayer.h"
#include "ObjectGrid.h"
#include "GridGenerator.h"

using namespace irr;
using namespace gui;

class ObjectDebugInfo : public Object
{
    public:
        ObjectDebugInfo(SContext* cont);
        virtual ~ObjectDebugInfo();

        virtual void onMessage(SMessage msg);

    private:
        IGUIFont* Font;

        core::vector3d<f64> Accel;

        ObjectPlayer* Player;
        scene::ICameraSceneNode* FPSCam;
};

//#endif // DEBUG_INFO

#endif // OBJECTDEBUGINFO_H
