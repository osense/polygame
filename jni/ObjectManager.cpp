#include "ObjectManager.h"

ObjectManager::ObjectManager(SContext* cont) : Object(cont)
{
    Name = "ObjectManager";
    debugLog(getName() + " spawned");
    setPersistent(true);

    registerObserver(this);
}

ObjectManager::~ObjectManager()
{
    debugLog(getName() + " died");
}

void ObjectManager::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_SPAWNED)
    {
        registerObserver(msg.Dispatcher);

        debugLog(msg.Dispatcher->getName() + " spawned");
    }
    else if (msg.Type == EMT_OBJ_DIED)
    {
        unregisterObserver(msg.Dispatcher);

        debugLog(msg.Dispatcher->getName() + " died");
    }
}


void ObjectManager::clear(bool persistent)
{
    // start from i = 1; don't delete ourselves :)
    for (u32 i = 1; i < Observers.size();)
    {
        if (!Observers[i]->isPersistent() || persistent)
        {
            delete Observers[i];
        }
        else
            i++;
    }
}

Object* ObjectManager::getObjectFromName(core::stringc name, Object* start) const
{
    u32 numObjs = Observers.size();
    u32 startIdx = start == 0 ? 0 : Observers.binary_search(start);

    for (u32 i = startIdx; i < numObjs; i++)
    {
        if (Observers[i]->getName() == name)
            return Observers[i];
    }

    return 0;
}

irr::core::stringc ObjectManager::getObjectsString() const
{
    irr::core::stringc str;

    u32 numObservers = Observers.size();
    for (u32 i = 0; i < numObservers; i++)
    {
        str += Observers[i]->getName();
        str += "\n";
    }

    return str;
}
