#include "ObjectHUD.h"

ObjectHUD::ObjectHUD(SContext* cont) : Object(cont),
    HUDColor(video::SColor(255, 255, 255, 255))
{
    Name = "ObjectHUD";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    core::rect<s32> distTextPos(20, 420, 200, 450);
    scaleGUIRect(distTextPos, Context->GUIScale);
    DistTravelledText = Context->Device->getGUIEnvironment()->addStaticText(L"", distTextPos);
    DistTravelledText->setOverrideColor(HUDColor);
}

ObjectHUD::~ObjectHUD()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    if (DistTravelledText)
        DistTravelledText->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectHUD::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        DT = msg.Position.Z;

        core::stringw dtText(floor32(DT*100) / 100.0);
        dtText = dtText.subString(0, dtText.findLastChar(L"123456789", 9) + 1);
        DistTravelledText->setText(dtText.c_str());
    }
    else if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            msg.Dispatcher->registerObserver(this);
    }
    else if (msg.Type == EMT_OBJ_DIED)
    {
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            msg.Dispatcher->unregisterObserver(this);
    }
}
