#ifndef OBJECTEVENTRECEIVER_H
#define OBJECTEVENTRECEIVER_H

#include <irrlicht.h>
#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#endif // _IRR_ANDROID_PLATFORM_

#include "Object.h"
#include "ObjectManager.h"
#include "Settings.h"
#include "circular_buffer.h"

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
    void transformAccelInput(const f64* canVec, f64* worldVec) const;
    
    circular_buffer<core::vector3df> AccelSamples;
};

#endif // OBJECTEVENTRECEIVER_H
