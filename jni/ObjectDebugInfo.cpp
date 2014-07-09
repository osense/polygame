#include "ObjectDebugInfo.h"

ObjectDebugInfo::ObjectDebugInfo(SContext* cont) : Object (cont)
{
    Name = "ObjectDebugInfo";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Font = Context->Device->getGUIEnvironment()->getBuiltInFont();// Context->Device->getGUIEnvironment()->getFont("gui/asap.xml");

#ifdef DEBUG_GYRO
    Accel = core::vector2d<f64>(0);
#endif

#ifdef DEBUG_PLAYER
    Player = 0;
#endif // DEBUG_PLAYER
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
        dbg += "\nMeshes: ";
        dbg += Context->Device->getSceneManager()->getMeshCache()->getMeshCount();

#ifdef DEBUG_PLAYER
        if (Player)
        {
            dbg += "\nSpeed: ";
            dbg += Player->getSpeed();
        }
#endif

        Font->draw(dbg.c_str(), core::rect<s32>(10, 10, 400, 200), video::SColor(255, 255, 255, 255));

#ifdef DEBUG_ACC
        core::dimension2d<u32> screenSize = Context->Device->getVideoDriver()->getScreenSize();
        f32 lineL = 50 * Context->GUIScale;

        core::position2d<s32> lineStart, lineEnd;
        lineStart.X = screenSize.Width - (75*Context->GUIScale);
        lineStart.Y = 25*Context->GUIScale;
        lineEnd.X = lineL * cos(-Accel.Y*0.1+1.57) + lineStart.X;
        lineEnd.Y = lineL * sin(-Accel.Y*0.1+1.57) + lineStart.Y;
        Context->Device->getVideoDriver()->draw2DLine(lineStart, lineEnd);
        core::stringc accStr = "Accelerometer\n    ";
        accStr += "Y: ";
        accStr += (s32)Accel.Y;
        Font->draw(accStr.c_str(), core::rect<s32>(lineStart.X, 10, 50, 20), video::SColor(255, 255, 255, 255));
#endif
    }
#ifdef DEBUG_ACC
    else if (msg.Type == EMT_ACC)
    {
        Accel.X = msg.Acc.X;
        Accel.Y = msg.Acc.Y;
        Accel.Z = msg.Acc.Z;
    }
#endif
    else if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectUpdater")
            msg.Dispatcher->registerObserver(this);

#ifdef DEBUG_PLAYER
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            Player = static_cast<ObjectPlayer*>(msg.Dispatcher);
#endif

#ifdef DEBUG_ACC
        if (msg.Dispatcher->getName() == "ObjectEventReceiver")
            msg.Dispatcher->registerObserver(this);
#endif
    }
    else if (msg.Type == EMT_OBJ_DIED)
    {
#ifdef DEBUG_PLAYER
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            Player = 0;

#endif // DEBUG_PLAYER
    }
}
