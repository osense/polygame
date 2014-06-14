#ifndef SMESSAGE_H
#define SMESSAGE_H

#include <irrlicht.h>

class Object;

enum E_MESSAGE_TYPE
{
    EMT_NONE = 0,
    EMT_UPDATE,
    EMT_OBJ_SPAWNED,
    EMT_OBJ_DIED,
    EMT_OBJ_POS,
    EMT_INPUT,
    EMT_GYRO,
    EMT_GUI,
    EMT_DAMAGE
};

struct SMessage
{
    E_MESSAGE_TYPE Type;
    Object* Dispatcher;

    struct GUIMessage
    {
        irr::gui::IGUIElement* Caller;
        irr::gui::EGUI_EVENT_TYPE EventType;
    };

    struct InputMessage
    {
        irr::ETOUCH_INPUT_EVENT Type;
        irr::s32 X, Y;
        irr::u32 ID;
    };

    struct GyroMessage
    {
        irr::f64 Roll, Pitch;
    };

    struct UpdateMessage
    {
        irr::u32 Delta;
    };

    struct ObjectPositionMessage
    {
        irr::f32 X, Y, Z;
    };

    union
    {
        GUIMessage GUI;
        InputMessage Input;
        GyroMessage Gyro;
        UpdateMessage Update;
        ObjectPositionMessage Position;
    };


    SMessage(Object* disp, E_MESSAGE_TYPE type)
    {
        Dispatcher = disp;
        Type = type;
    }
};

#endif
