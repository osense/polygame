#ifndef OBJECTDEBUGINFO_H
#define OBJECTDEBUGINFO_H

#include "Object.h"
#include "ObjectManager.h"
#include <irrlicht.h>

#include "ObjectEventReceiver.h"
#include "ObjectPlayer.h"

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
};

#endif // OBJECTDEBUGINFO_H
