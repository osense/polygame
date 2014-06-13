#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <irrlicht.h>

#include "SContext.h"
#include "SMessage.h"

using namespace irr;

class Object
{
    public:
        Object(SContext* cont);
        virtual ~Object();

        void broadcastMessage(SMessage msg);
        void registerObserver(Object* obj);
        void unregisterObserver(Object* obj);

        void setPersistent(bool pers);
        bool isPersistent() const;

        core::stringc getName() const;
        virtual void debugLog(core::stringc text) const;

        virtual void onMessage(SMessage msg) = 0;


    protected:
        SContext* Context;
        core::array<Object*> Observers;

        bool Persistent;
        core::stringc Name;
};

#endif // GAMEOBJECT_H
