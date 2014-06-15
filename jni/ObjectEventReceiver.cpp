#include "ObjectEventReceiver.h"

ObjectEventReceiver::ObjectEventReceiver(SContext* cont) : Object(cont)
{
    Name = "ObjectEventReceiver";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));
    setPersistent(true);

    Context->Device->setEventReceiver(this);
}

ObjectEventReceiver::~ObjectEventReceiver()
{
    Context->Device->setEventReceiver(0);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectEventReceiver::onMessage(SMessage msg)
{

}

void ObjectEventReceiver::debugLog(core::stringc text) const
{
#ifdef DEBUG_INPUT
    SContext->Device->getLogger()->log(getName().c_str(), text.c_str());
#endif
}


bool ObjectEventReceiver::OnEvent(const SEvent& event)
{
    if (event.EventType == EET_GUI_EVENT)
    {
        SMessage msg(this, EMT_GUI);
        msg.GUI.EventType = event.GUIEvent.EventType;
        msg.GUI.Caller = event.GUIEvent.Caller;

        debugLog("broadcasting a gui message...");
        broadcastMessage(msg);
        return true;
    }
    #ifndef DEBUG_GLES
    else if (event.EventType == EET_TOUCH_INPUT_EVENT)
    {
        SMessage msg(this, EMT_INPUT);
        msg.Input.Type = event.TouchInput.Event;
        msg.Input.ID = event.TouchInput.ID;
        msg.Input.X = event.TouchInput.X;
        msg.Input.Y = event.TouchInput.Y;

        broadcastMessage(msg);
        return true;
    }
    #else
    else if (event.EventType == EET_MOUSE_INPUT_EVENT)
    {
        SMessage msg(this, EMT_INPUT);

        if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            msg.Input.Type = ETIE_PRESSED_DOWN;
        else if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
            msg.Input.Type = ETIE_LEFT_UP;
        else if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
            msg.Input.Type = ETIE_MOVED;

        msg.Input.ID = 0;
        msg.Input.X = event.MouseInput.X;
        msg.Input.Y = event.MouseInput.Y;

        broadcastMessage(msg);
        return false;
    }
    #endif
    else if (event.EventType == EET_ACCELEROMETER_EVENT)
    {
        SMessage msg(this, EMT_GYRO);
        msg.Gyro.Roll = event.AccelerometerEvent.X;
        msg.Gyro.Pitch = event.AccelerometerEvent.Y;

        broadcastMessage(msg);
        return true;
    }

    return false;
}
