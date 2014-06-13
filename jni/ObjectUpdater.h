#ifndef GAMEOBJECTUPDATER_H
#define GAMEOBJECTUPDATER_H

#include <irrlicht.h>
#include "Object.h"
#include "ObjectManager.h"
#include "SContext.h"


class ObjectUpdater : public Object
{
    public:
        ObjectUpdater(SContext* cont);
        virtual ~ObjectUpdater();

        virtual void onMessage(SMessage msg);

        void broadcastUpdate(irr::u32 tDelta);
};

#endif // GAMEOBJECTUPDATER_H
