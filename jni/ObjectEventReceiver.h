#ifndef OBJECTEVENTRECEIVER_H
#define OBJECTEVENTRECEIVER_H

#include <irrlicht.h>
#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#endif // _IRR_ANDROID_PLATFORM_

#include "Object.h"
#include "ObjectManager.h"

using namespace irr;

class ObjectEventReceiver : public Object, public IEventReceiver
{
    public:
        ObjectEventReceiver(SContext* cont);
        virtual ~ObjectEventReceiver();

        virtual void onMessage(SMessage msg);
        virtual void debugLog(core::stringc text) const;

        virtual bool OnEvent(const SEvent& event);

    private:
};

#endif // OBJECTEVENTRECEIVER_H
