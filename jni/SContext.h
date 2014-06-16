#ifndef SContext_H_INCLUDED
#define SContext_H_INCLUDED


#define DEBUG_FPS
#define DEBUG_ACC
#define DEBUG_PLAYER
#define DEBUG_OBJS
//#define DEBUG_GRID

#define DESKTOP_WND_X 854
#define DESKTOP_WND_Y 480

namespace irr
{
    class IrrlichtDevice;
}

class Object;
class ObjectManager;


struct SContext
{
    irr::IrrlichtDevice* Device;

    Object* State;
    ObjectManager* ObjManager;

    irr::f32 GUIScale;
};

#endif // SContext_H_INCLUDED
