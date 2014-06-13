#include "Object.h"

Object::Object(SContext* cont)
{
    Context = cont;
    Name = "UnknownObject";
    setPersistent(false);
}

Object::~Object()
{
    //dtor
}

void Object::broadcastMessage(SMessage msg)
{
    for (u32 i = 0; i < Observers.size(); i++)
    {
        Observers[i]->onMessage(msg);
    }
}

void Object::registerObserver(Object* obj)
{
    Observers.push_back(obj);
}

void Object::unregisterObserver(Object* obj)
{
    u32 numObservers = Observers.size();
    for (u32 i = 0; i < numObservers; i++)
    {
        if (Observers[i] == obj)
        {
            Observers.erase(i);
            return;
        }
    }
}

void Object::setPersistent(bool pers)
{
    Persistent = pers;
}

bool Object::isPersistent() const
{
    return Persistent;
}

irr::core::stringc Object::getName() const
{
    return Name;
}

void Object::debugLog(core::stringc text) const
{
#ifdef DEBUG_OBJS
    SContext->Device->getLogger()->log(getName().c_str(), text.c_str());
#endif
}
