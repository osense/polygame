#ifndef OBJECTCINEMATICCUBES_H
#define OBJECTCINEMATICCUBES_H

#include "Object.h"
#include "ObjectManager.h"
#include "SMaterials.h"
#include <irrlicht.h>

using namespace irr;

class ObjectCinematicCubes : public Object
{
public:
    ObjectCinematicCubes(SContext* cont);

    virtual ~ObjectCinematicCubes();

    virtual void onMessage(SMessage msg);

    void setVisible(bool vis);

    bool isVisible() const;

private:
    core::vector3df getRandomPointOnCircle(f32 cRadius);

    static constexpr u32 MaxCubes = 4;
    static constexpr f32 SpawnDepth = 3;
    static constexpr f32 SpawnDepthRandomOffset = 1.5;
    static constexpr f32 MaxRotationSpeed = 0.25;
    static constexpr u32 TravelTime = 15000;
    static constexpr u32 TravelTimeRandomOffset = 5000;

    f32 AspectRatio;
    scene::ISceneNode* Parent;
    core::array<scene::IMeshSceneNode*> Cubes;
};

#endif // OBJECTCINEMATICCUBES_H
