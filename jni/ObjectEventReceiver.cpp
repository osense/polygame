#include "ObjectEventReceiver.h"

ObjectEventReceiver::ObjectEventReceiver(SContext* cont) : Object(cont),
    AccelSamples(Context->Sets->AccelSamples)
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
    Context->Device->getLogger()->log(getName().c_str(), text.c_str());
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
    else if (event.EventType == EET_KEY_INPUT_EVENT)
    {
        SMessage msg(this, EMT_KEY);

        if (event.KeyInput.Key == KEY_ESCAPE && event.KeyInput.PressedDown == false)
        {
            msg.Key.Code = irr::KEY_BACK;
        }
        else
        {
            msg.Key.Code = event.KeyInput.Key;
            msg.Key.Pressed = event.KeyInput.PressedDown;
        }

        broadcastMessage(msg);
    }
#endif
    else if (event.EventType == EET_ACCELEROMETER_EVENT)
    {
        const f64 canonAcc[] = {event.AccelerometerEvent.X, event.AccelerometerEvent.Y, event.AccelerometerEvent.Z};
        f64 worldAcc[3];
        transformAccelInput(canonAcc, worldAcc);

        AccelSamples.push_back(core::vector3df(-worldAcc[0] + Context->Sets->AccelXBias, worldAcc[1], worldAcc[2]));

        core::vector3df total(0);
        for (u32 i = 0; i < AccelSamples.getSize(); i++)
            total += AccelSamples[i];

        total /= AccelSamples.getSize();

        SMessage msg(this, EMT_ACC);
        msg.Acc.X = total.X;
        msg.Acc.Y = total.Y;
        msg.Acc.Z = total.Z;
        broadcastMessage(msg);
        return true;
    }
    else if (event.EventType == EET_SYSTEM_EVENT && event.SystemEvent.EventType == ESET_ANDROID_CMD)
    {
        SMessage msg(this, EMT_APPCOMMAND);
        msg.AppCommand.Cmd = event.SystemEvent.AndroidCmd.Cmd;

        broadcastMessage(msg);
        return false;
    }

    return false;
}

void ObjectEventReceiver::transformAccelInput(const f64* canVec, f64* worldVec) const
{
    struct AxisSwap
    {
        signed char negateX;
        signed char negateY;
        signed char xSrc;
        signed char ySrc;
    };
    static const AxisSwap axisSwap[] =
    {
        { 1, 1, 0, 1 }, // ROTATION_0
        {-1, 1, 1, 0 }, // ROTATION_90
        {-1, -1, 0, 1 },// ROTATION_180
        { 1, -1, 1, 0 } // ROTATION_270
    };

    const AxisSwap& as = axisSwap[u32(Context->ScreenRotation)];
    worldVec[0] = (f32)as.negateX * canVec[as.xSrc];
    worldVec[1] = (f32)as.negateY * canVec[as.ySrc];
    worldVec[2] = canVec[2];
}
