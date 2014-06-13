#ifndef GAMEOBJECTMANAGER_H
#define GAMEOBJECTMANAGER_H


#include "Object.h"
#include <irrlicht.h>

using namespace irr;

class ObjectManager : public Object
{
    public:
        ObjectManager(SContext* cont);
        virtual ~ObjectManager();

        virtual void onMessage(SMessage msg);

        void clear(bool persistent = false);
        Object* getObjectFromName(core::stringc name, Object* start = 0) const;
        core::stringc getObjectsString() const;
};

#endif // GAMEOBJECTMANAGER_H
