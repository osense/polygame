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
    /*else if (event.EventType == EET_TOUCH_INPUT_EVENT)
    {
        SMessage msg(this, EMT_INPUT);
        msg.KeyCode = event.KeyInput.Key;

        broadcastMessage(msg);
        return true;
    }*/
    else if (event.EventType == EET_GYROSCOPE_EVENT)
    {
        SMessage msg(this, EMT_GYRO);
        msg.Gyro.Roll = event.GyroscopeEvent.X;
        msg.Gyro.Pitch = event.GyroscopeEvent.Y;

        broadcastMessage(msg);
        return true;
    }

    return false;
}
