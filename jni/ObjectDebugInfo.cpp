#include "ObjectDebugInfo.h"

ObjectDebugInfo::ObjectDebugInfo(SContext* cont) : Object (cont)
{
    Name = "ObjectDebugInfo";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    #ifdef DEBUG_GYRO
    Gyro = core::vector2d<f64>(90);
    #endif
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

        #ifdef DEBUG_PLAYER
        dbg += "\nSpeed: ";
        dbg += Player->getSpeed();
        #endif

        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(dbg.c_str(), core::rect<s32>(10, 10, 400, 200), video::SColor(255, 255, 255, 255));

        #ifdef DEBUG_GYRO
        core::dimension2d<u32> screenSize = Context->Device->getVideoDriver()->getScreenSize();
        f32 lineL = 50 * Context->GUIScale;

        core::position2d<s32> rollStart, rollEnd;
        rollStart.X = screenSize.Width - 50;
        rollStart.Y = 30;
        //rollEnd.X = lineL * cos(core::degToRad(Gyro.X)) + rollStart.X;
        //rollEnd.Y = lineL * sin(core::degToRad(Gyro.X)) + rollStart.Y;
        rollEnd.X = lineL * cos(Gyro.X) + rollStart.X;
        rollEnd.Y = lineL * sin(Gyro.X) + rollStart.Y;

        core::stringw rollS = "Roll: ";
        rollS += (int)Gyro.X;
        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(rollS.c_str(), core::rect<s32>(screenSize.Width - 65, 10, screenSize.Width, 30),
                                                                         video::SColor(255, 255, 255, 255));
        Context->Device->getVideoDriver()->draw2DLine(rollStart, rollEnd);

        #endif
    }
    #ifdef DEBUG_GYRO
    else if (msg.Type == EMT_GYRO)
    {
        Gyro.X = msg.Gyro.Roll;
        Gyro.Y = msg.Gyro.Pitch;
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

        #ifdef DEBUG_GYRO
        if (msg.Dispatcher->getName() == "ObjectEventReceiver")
            msg.Dispatcher->registerObserver(this);
        #endif
    }
}
