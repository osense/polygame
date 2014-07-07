#ifndef OBJECTDEBUGINFO_H
#define OBJECTDEBUGINFO_H

#include "Object.h"
#include "ObjectManager.h"
#include <irrlicht.h>


#ifdef DEBUG_GYRO
#include "ObjectEventReceiver.h"
#endif

#ifdef DEBUG_PLAYER
#include "ObjectPlayer.h"
#endif

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

        #ifdef DEBUG_ACC
        core::vector3d<f64> Accel;
        #endif

        #ifdef DEBUG_PLAYER
        ObjectPlayer* Player;
        #endif
};

#endif // OBJECTDEBUGINFO_H
