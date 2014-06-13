#include "ObjectDebugInfo.h"

ObjectDebugInfo::ObjectDebugInfo(SContext* cont) : Object (cont)
{
    Name = "ObjectDebugInfo";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Object* updater = Context->ObjManager->getObjectFromName("ObjectUpdater");
    if (updater)
        updater->registerObserver(this);
}

ObjectDebugInfo::~ObjectDebugInfo()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectDebugInfo::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        core::stringw dbg = "FPS: ";
        dbg += Context->Device->getVideoDriver()->getFPS();
        dbg += "\nTris: ";
        dbg += Context->Device->getVideoDriver()->getPrimitiveCountDrawn();

        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(dbg.c_str(),
                                                                         core::rect<s32>(10, 10, 400, 200),
                                                                         video::SColor(255, 255, 255, 255));
    }
}
