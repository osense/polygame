#ifndef SMESSAGE_H
#define SMESSAGE_H

#include <irrlicht.h>

class Object;
namespace Json
{
    class Value;
}

enum E_MESSAGE_TYPE
{
    EMT_NONE = 0,
    EMT_UPDATE,
    EMT_SERIALIZE,
    EMT_DESERIALIZE,
    EMT_OBJ_SPAWNED,
    EMT_OBJ_DIED,
    EMT_OBJ_POS,
    EMT_GRID_REGENED,
    EMT_PLAYER_FEEDBACK,
    EMT_INPUT,
    EMT_KEY,
    EMT_ACC,
    EMT_GUI,
    EMT_PLAYER_CRASHED,
    EMT_PLAYER_CUBED
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

    struct KeyMessage
    {
        irr::EKEY_CODE Code;
        bool Pressed;
    };

    struct AccMessage
    {
        irr::f64 X, Y, Z;
    };

    struct UpdateMessage
    {
        irr::u32 Delta;
        irr::f32 fDelta;
    };

    struct SerializeMessage
    {
        Json::Value* Root;
    };

    struct ObjectPositionMessage
    {
        irr::f32 X, Y, Z;
    };

    struct PlayerFeedbackMessage
    {
        irr::f32 Height;
        irr::f32 GridAngle;
    };

    union
    {
        GUIMessage GUI;
        InputMessage Input;
        KeyMessage Key;
        AccMessage Acc;
        UpdateMessage Update;
        SerializeMessage SData;
        ObjectPositionMessage Position;
        PlayerFeedbackMessage PlayerFeedback;
    };


    SMessage(Object* disp, E_MESSAGE_TYPE type)
    {
        Dispatcher = disp;
        Type = type;
    }
};

#endif
