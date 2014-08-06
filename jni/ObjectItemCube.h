#ifndef OBJECTITEMCUBE_H
#define OBJECTITEMCUBE_H

#include "ObjectItem.h"
#include "ObjectManager.h"
#include "GeometryGenerator.h"
#include "SMaterials.h"
#include "ObjectGrid.h"
#include "ShaderCBGridBack.h"
#include <irrlicht.h>

using namespace irr;

class ObjectItemCube : public ObjectItem
{
    public:
        ObjectItemCube(SContext* cont, core::vector3df pos);

        virtual ~ObjectItemCube();

        virtual void onMessage(SMessage msg);

        scene::IMesh* getCubeMesh();

    private:
        scene::ISceneNode* Node;
        f32 EffectCounter;

        const bool Filled = false;
        const f32 CubeSize = 0.25;
        const f32 RotSpeed = 0.025;
        const f32 DestroyDist = 2;
        const f32 TimeActive = 15;
        const f32 TimeFadein = 1.5;
};

#endif // OBJECTITEMCUBE_H
