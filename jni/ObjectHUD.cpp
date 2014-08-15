#include "ObjectHUD.h"

ObjectHUD::ObjectHUD(SContext* cont) : Object(cont),
    DistTravelledText(0)
{
    Name = "ObjectHUD";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    core::rect<s32> distTextPos(20, 420, 200, 450);
    scaleGUIRect(distTextPos, Context->GUIScale);
    DistTravelledText = Context->Device->getGUIEnvironment()->addStaticText(L"", distTextPos);
    DistTravelledText->setOverrideColor(video::SColor(255, 255, 255, 255));

    // prepare energy indicator
    VLineSegment = Context->Device->getVideoDriver()->getTexture("textures/line_v_bright.png");
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

        if (DistTravelledText)
        {
            core::stringw dtText(floor32(DT*100) / 100.0);
            dtText = dtText.subString(0, dtText.findLastChar(L"123456789", 9) + 1);
            DistTravelledText->setText(dtText.c_str());
        }

        ObjectPlayer* pl = static_cast<ObjectPlayer*>(Context->ObjManager->getObjectFromName("ObjectPlayer"));
        u32 pixelsToDraw = pl->getEnergy() * PPE;

        if (EnergyPositions.size() < pixelsToDraw)
        {
            for (u32 i = EnergyPositions.size(); i < pixelsToDraw; i++)
            {
                EnergyPositions.push_back(core::position2d<s32>(EnergyX*Context->GUIScale.X + i, EnergyY*Context->GUIScale.Y));
                EnergyRects.push_back(core::rect<s32>(core::vector2d<s32>(0, 0), VLineSegment->getSize()));
            }
        }
        else if (EnergyPositions.size() > pixelsToDraw)
        {
            EnergyPositions.erase(pixelsToDraw, EnergyPositions.size() - pixelsToDraw);
            EnergyRects.erase(pixelsToDraw, EnergyRects.size() - pixelsToDraw);
        }
    }
    else if (msg.Type == EMT_UPDATE)
    {
        Context->Device->getVideoDriver()->draw2DImageBatch(VLineSegment, EnergyPositions, EnergyRects, 0, video::SColor(255, 255, 255, 255), true);
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
