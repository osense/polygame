#include "ObjectUpdater.h"

ObjectUpdater::ObjectUpdater(SContext* cont) : Object(cont)
{
    Name = "ObjectUpdater";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));
}

ObjectUpdater::~ObjectUpdater()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectUpdater::onMessage(SMessage msg)
{

}

void ObjectUpdater::broadcastUpdate(irr::u32 tDelta)
{
    SMessage msg(this, EMT_UPDATE);
    msg.Update.Delta = tDelta;
    broadcastMessage(msg);
}
