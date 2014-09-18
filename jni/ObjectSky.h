#ifndef OBJECTSKY_H
#define OBJECTSKY_H

#include "Object.h"
#include "ObjectManager.h"
#include "SMaterials.h"

#include <irrlicht.h>

using namespace irr;


class ObjectSky : public Object
{
public:

    ObjectSky(SContext* cont);
    ~ObjectSky();
    virtual void onMessage(SMessage msg);

private:
    scene::ISceneNode* SkyNode;
    f32 CurrentRotSpeed;

    static constexpr f32 RotSpeed = 0.0003;
};

#endif // OBJECTSKY_H
