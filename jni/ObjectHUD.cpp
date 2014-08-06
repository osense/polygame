#include "ObjectHUD.h"

ObjectHUD::ObjectHUD(SContext* cont) : Object(cont),
    DistTravelledText(0)
{
    Name = "ObjectHUD";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    core::rect<s32> distTextPos(20, 420, 200, 450);
    scaleGUIRect(distTextPos, Context->GUIScale);
    DistTravelledText = Context->Device->getGUIEnvironment()->addStaticText(L"", distTextPos);
    DistTravelledText->setOverrideColor(video::SColor(255, 255, 255, 255));
}

ObjectHUD::~ObjectHUD()
{
    if (DistTravelledText)
        DistTravelledText->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectHUD::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        DT = msg.Position.Z;

        if (DistTravelledText)
        {
            core::stringw dtText(floor32(DT*100) / 100.0);
            dtText = dtText.subString(0, dtText.findLastChar(L"123456789", 9) + 1);
            DistTravelledText->setText(dtText.c_str());
        }
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        DistTravelledText->remove();
        DistTravelledText = 0;
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
