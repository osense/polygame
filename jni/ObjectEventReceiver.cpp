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

        // fix for irrlicht gui
        SEvent fakeMouseEvent;
        fakeMouseEvent.EventType = EET_MOUSE_INPUT_EVENT;
        fakeMouseEvent.MouseInput.X = event.TouchInput.X;
        fakeMouseEvent.MouseInput.Y = event.TouchInput.Y;
        fakeMouseEvent.MouseInput.Shift = false;
        fakeMouseEvent.MouseInput.Control = false;
        fakeMouseEvent.MouseInput.ButtonStates = 0;

        switch ( event.TouchInput.Event)
        {
        case ETIE_PRESSED_DOWN:
        {
            fakeMouseEvent.MouseInput.Event = EMIE_LMOUSE_PRESSED_DOWN;
            break;
        }
        case ETIE_MOVED:
        {
            fakeMouseEvent.MouseInput.Event = EMIE_MOUSE_MOVED;
            fakeMouseEvent.MouseInput.ButtonStates = EMBSM_LEFT;
            break;
        }
        case ETIE_LEFT_UP:
        {
            fakeMouseEvent.MouseInput.Event = EMIE_LMOUSE_LEFT_UP;
            break;
        }
        default:
            break;
        }
        Context->Device->postEventFromUser(fakeMouseEvent);


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
        SMessage msg(this, EMT_ACC);
        msg.Acc.X = event.AccelerometerEvent.X;
        msg.Acc.Y = event.AccelerometerEvent.Y;
        msg.Acc.Z = event.AccelerometerEvent.Z;

        broadcastMessage(msg);
        return true;
    }

    return false;
}
