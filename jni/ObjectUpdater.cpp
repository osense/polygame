#include "ObjectUpdater.h"

ObjectUpdater::ObjectUpdater(SContext* cont) : Object(cont)
{
    Name = "ObjectUpdater";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);
}

ObjectUpdater::~ObjectUpdater()
{
    Object* eventR = Context->ObjManager->getObjectFromName("ObjectEventReceiver");
    if (eventR)
        eventR->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectUpdater::onMessage(SMessage msg)
{
#ifdef _IRR_ANDROID_PLATFORM_
    if (msg.Type == EMT_APPCOMMAND && msg.AppCommand.Cmd == APP_CMD_LOST_FOCUS)
    {
        Context->Device->getTimer()->stop();
        debugLog("PAUSED");
    }
    else if (msg.Type == EMT_APPCOMMAND && msg.AppCommand.Cmd == APP_CMD_GAINED_FOCUS)
    {
        Context->Device->getTimer()->start();
        debugLog("RESUMED");
    }
#endif // _IRR_ANDROID_PLATFORM_
}

void ObjectUpdater::broadcastUpdate(irr::u32 tDelta)
{
    SMessage msg(this, EMT_UPDATE);
    msg.Update.Delta = tDelta;
    msg.Update.fDelta = tDelta / 1000.0;
    broadcastMessage(msg);
}
