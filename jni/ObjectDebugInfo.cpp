#include "ObjectDebugInfo.h"

ObjectDebugInfo::ObjectDebugInfo(SContext* cont) : Object (cont)
{
    Name = "ObjectDebugInfo";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Font = Context->Device->getGUIEnvironment()->getBuiltInFont();

    Accel = core::vector3d<f64>(0);
    Player = 0;
    FPSCam = 0;
    ObjectCount = 0;
}

ObjectDebugInfo::~ObjectDebugInfo()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);
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
        dbg += "\nItems: ";
        dbg += ObjectCount;

        if (Player)
        {
            dbg += "\nSpeed: ";
            dbg += Player->getSpeed();
            dbg += "\nHeight: ";
            dbg += Player->getHeight();
        }

        ObjectGrid* grid = static_cast<ObjectGrid*>(Context->ObjManager->getObjectFromName("ObjectGrid"));
        if (grid)
        {
            dbg += "\nTerrain type: ";
            switch (grid->getGenerator().getType())
            {
            case EGT_PLAINS:
                dbg += "PLAINS";
                break;
            case EGT_HILLS:
                dbg += "HILLS";
                break;
            case EGT_CANYONS:
                dbg += "CANYONS";
                break;
            case EGT_WALLS:
                dbg += "WALLS";
                break;
            default:
                dbg += "ERROR";
            }

            dbg += "\nDifficulty: ";
            dbg += grid->getGenerator().getDifficulty();
        }

        ObjectGridCinematicLines* gridLines = static_cast<ObjectGridCinematicLines*>(Context->ObjManager->getObjectFromName("ObjectGridCinematicLines"));
        if (gridLines)
        {
            dbg += "\nCin. lines: ";
            dbg += gridLines->getLineCount();
        }

        Font->draw(dbg.c_str(), core::rect<s32>(10, 10, 400, 200), video::SColor(255, 255, 255, 255));

        core::dimension2d<u32> screenSize = Context->Device->getVideoDriver()->getScreenSize();
        f32 lineL = 50 * Context->GUIScale.Y;

        core::position2d<s32> lineStart, lineEnd;
        lineStart.X = screenSize.Width - (75*Context->GUIScale.X);
        lineStart.Y = 25*Context->GUIScale.Y;
        lineEnd.X = lineL * cos(-Accel.X*0.1+1.57) + lineStart.X;
        lineEnd.Y = lineL * sin(-Accel.X*0.1+1.57) + lineStart.Y;
        Context->Device->getVideoDriver()->draw2DLine(lineStart, lineEnd);
        core::stringc accStr = "Accelerometer\n    ";
        accStr += "X: ";
        accStr += (s32)Accel.X;
        Font->draw(accStr.c_str(), core::rect<s32>(lineStart.X, 10, 50, 20), video::SColor(255, 255, 255, 255));
    }
    else if (msg.Type == EMT_ACC)
    {
        Accel.X = msg.Acc.X;
        Accel.Y = msg.Acc.Y;
        Accel.Z = msg.Acc.Z;
    }
    else if (msg.Type == EMT_KEY)
    {
        if (!Player)
            return;

        if (!msg.Key.Pressed && msg.Key.Code == KEY_KEY_V)
        {
            if (!FPSCam)
            {
                FPSCam = Context->Device->getSceneManager()->addCameraSceneNodeFPS(0, 100, 0.01);
                FPSCam->setPosition(Player->getCamera()->getPosition());
                FPSCam->setFarValue(Player->getCamera()->getFarValue());
                FPSCam->setNearValue(Player->getCamera()->getNearValue());
                Context->TimeScale = 0;
            }
            else
            {
                FPSCam->remove();
                FPSCam = 0;
                Context->Device->getSceneManager()->setActiveCamera(Player->getCamera());
                Context->TimeScale = 1;
            }
        }
    }
    else if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectItemCube")
            ObjectCount++;

        else if (msg.Dispatcher->getName() == "ObjectUpdater")
            msg.Dispatcher->registerObserver(this);

        else if (msg.Dispatcher->getName() == "ObjectPlayer")
            Player = static_cast<ObjectPlayer*>(msg.Dispatcher);

        else if (msg.Dispatcher->getName() == "ObjectEventReceiver")
            msg.Dispatcher->registerObserver(this);
    }
    else if (msg.Type == EMT_OBJ_DIED)
    {
        if (msg.Dispatcher->getName() == "ObjectItemCube")
            ObjectCount--;

        else if (msg.Dispatcher->getName() == "ObjectPlayer")
            Player = 0;
    }
}
