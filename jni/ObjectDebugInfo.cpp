#include "ObjectDebugInfo.h"

ObjectDebugInfo::ObjectDebugInfo(SContext* cont) : Object (cont)
{
    Name = "ObjectDebugInfo";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    #ifdef DEBUG_GYRO
    Accel = core::vector2d<f64>(0);
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

        #ifdef DEBUG_ACC
        core::dimension2d<u32> screenSize = Context->Device->getVideoDriver()->getScreenSize();
        f32 lineL = 50 * Context->GUIScale;

        core::position2d<s32> XStart, XEnd;
        XStart.X = screenSize.Width - (150*Context->GUIScale);
        XStart.Y = 50*Context->GUIScale;
        XEnd.X = lineL * cos(Accel.X) + XStart.X;
        XEnd.Y = lineL * sin(Accel.X) + XStart.Y;
        Context->Device->getVideoDriver()->draw2DLine(XStart, XEnd);
        core::stringc Xstr = "X: ";
        Xstr += (s32)Accel.X;
        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(Xstr.c_str(), core::rect<s32>(XStart.X, 10, 50, 20), video::SColor(255, 255, 255, 255));

        core::position2d<s32> YStart, YEnd;
        YStart.X = screenSize.Width - (100*Context->GUIScale);
        YStart.Y = 50*Context->GUIScale;
        YEnd.X = lineL * cos(Accel.Y) + YStart.X;
        YEnd.Y = lineL * sin(Accel.Y) + YStart.Y;
        Context->Device->getVideoDriver()->draw2DLine(YStart, YEnd);
        core::stringc Ystr = "Y: ";
        Ystr += (s32)Accel.Y;
        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(Ystr.c_str(), core::rect<s32>(YStart.X, 10, 50, 20), video::SColor(255, 255, 255, 255));

        core::position2d<s32> ZStart, ZEnd;
        ZStart.X = screenSize.Width - (50*Context->GUIScale);
        ZStart.Y = 50*Context->GUIScale;
        ZEnd.X = lineL * cos(Accel.Z) + ZStart.X;
        ZEnd.Y = lineL * sin(Accel.Z) + ZStart.Y;
        Context->Device->getVideoDriver()->draw2DLine(ZStart, ZEnd);
        core::stringc Zstr = "Z: ";
        Zstr += (s32)Accel.Z;
        Context->Device->getGUIEnvironment()->getSkin()->getFont()->draw(Zstr.c_str(), core::rect<s32>(ZStart.X, 10, 50, 20), video::SColor(255, 255, 255, 255));

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
}
