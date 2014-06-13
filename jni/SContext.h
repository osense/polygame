#ifndef SContext_H_INCLUDED
#define SContext_H_INCLUDED

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
};

#endif // SContext_H_INCLUDED
