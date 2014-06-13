#ifndef OBJECTDEBUGINFO_H
#define OBJECTDEBUGINFO_H

#include "Object.h"
#include "ObjectManager.h"
#include <irrlicht.h>

using namespace irr;
using namespace gui;

class ObjectDebugInfo : public Object
{
    public:
        ObjectDebugInfo(SContext* cont);
        virtual ~ObjectDebugInfo();

        virtual void onMessage(SMessage msg);

    private:
};

#endif // OBJECTDEBUGINFO_H
