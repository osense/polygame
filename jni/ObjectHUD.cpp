#include "ObjectHUD.h"

ObjectHUD::ObjectHUD(SContext* cont) : Object(cont),
    HUDColor(video::SColor(255, 255, 255, 255)),
    FadeHUDColor(video::SColor(0, 255, 255, 255)),
    DistVisibility(EHEV_ON),
    EnergyVisibility(EHEV_ON)
{
    Name = "ObjectHUD";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    core::rect<s32> distTextPos(20, 420, 200, 450);
    scaleGUIRect(distTextPos, Context->GUIScale);
    DistTravelledText = Context->Device->getGUIEnvironment()->addStaticText(L"", distTextPos);
    DistTravelledText->setOverrideColor(HUDColor);

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

        core::stringw dtText(floor32(DT*100) / 100.0);
        dtText = dtText.subString(0, dtText.findLastChar(L"123456789", 9) + 1);
        DistTravelledText->setText(dtText.c_str());

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
        // HUD fading
        u32 flDT = core::floor32(DT);
        if (DistVisibility == EHEV_ON || DistVisibility == EHEV_OFF)
        {
            if (((flDT + DisplayDTFrom) % DisplayDTEvery) == 0)
            {
                DistVisibility = EHEV_FADEIN;
                DistTravelledText->setVisible(true);
                DTFadeCounter = DTFadeTime;
            }
            else if (((flDT - DisplayDTTo) % DisplayDTEvery) == 0 || (flDT == DisplayDTTo - DTFadeTime))
            {
                DistVisibility = EHEV_FADEOUT;
                DTFadeCounter = DTFadeTime;
            }
        }

        if (DistVisibility == EHEV_FADEIN)
        {
            video::SColor newCol;
            newCol = FadeHUDColor.getInterpolated(HUDColor, DTFadeCounter/DTFadeTime);
            DistTravelledText->setOverrideColor(newCol);

            DTFadeCounter -= msg.Update.fDelta;
            if (DTFadeCounter <= 0)
                DistVisibility = EHEV_ON;
        }
        else if (DistVisibility == EHEV_FADEOUT)
        {
            video::SColor newCol;
            newCol = HUDColor.getInterpolated(FadeHUDColor, DTFadeCounter/DTFadeTime);
            DistTravelledText->setOverrideColor(newCol);

            DTFadeCounter -= msg.Update.fDelta;
            if (DTFadeCounter <= 0)
            {
                DistVisibility = EHEV_OFF;
                DistTravelledText->setVisible(false);
            }
        }

        // energy fading
        video::SColor energyCol = HUDColor;
        ObjectPlayer* pl = static_cast<ObjectPlayer*>(Context->ObjManager->getObjectFromName("ObjectPlayer"));
        f32 energy = pl->getEnergy();
        f32 maxEnergy = pl->getMaxEnergy();
        if (EnergyVisibility == EHEV_OFF && (energy != maxEnergy))
        {
            energyCol = FadeHUDColor;
            EnergyVisibility = EHEV_FADEIN;
            EnergyFadeCounter = EnergyFadeTime;
        }
        else if (EnergyVisibility == EHEV_ON && (energy == maxEnergy))
        {
            EnergyVisibility = EHEV_WAIT_FADEOUT;
            EnergyFadeCounter = EnergyFadeTime;
        }
        else if (EnergyVisibility == EHEV_FADEIN)
        {
            energyCol = FadeHUDColor.getInterpolated(HUDColor, EnergyFadeCounter/EnergyFadeTime);
            EnergyFadeCounter -= msg.Update.fDelta;
            if (EnergyFadeCounter <= 0)
            {
                EnergyVisibility = EHEV_ON;
            }
        }
        else if (EnergyVisibility == EHEV_WAIT_FADEOUT)
        {
            EnergyFadeCounter -= msg.Update.fDelta;
            if (energy != maxEnergy)
            {
                EnergyVisibility = EHEV_ON;
            }
            else if (EnergyFadeCounter <= 0)
            {
                EnergyFadeCounter = EnergyFadeTime;
                EnergyVisibility = EHEV_FADEOUT;
            }
        }
        else if (EnergyVisibility == EHEV_FADEOUT)
        {
            energyCol = HUDColor.getInterpolated(FadeHUDColor, EnergyFadeCounter/EnergyFadeTime);
            EnergyFadeCounter -= msg.Update.fDelta;
            if (energy != maxEnergy)
            {
                EnergyVisibility = EHEV_FADEIN;
                EnergyFadeCounter = EnergyFadeTime - EnergyFadeCounter;
            }
            else if (EnergyFadeCounter <= 0)
            {
                EnergyVisibility = EHEV_OFF;
            }
        }

        if (EnergyVisibility != EHEV_OFF)
            Context->Device->getVideoDriver()->draw2DImageBatch(VLineSegment, EnergyPositions, EnergyRects, 0, energyCol, true);
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
