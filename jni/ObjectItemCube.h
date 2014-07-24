#ifndef OBJECTITEMCUBE_H
#define OBJECTITEMCUBE_H

#include "ObjectItem.h"
#include "ObjectManager.h"
#include "SMaterials.h"
#include "ShaderCBGrid.h"
#include "ShaderCBGridBack.h"
#include "ShaderCBCube.h"
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

        static const bool Filled = false;
        static const f32 CubeSize = 0.25;
        static const f32 RotSpeed = 0.025;
        static const f32 DestroyDist = 2;
        static const f32 TimeActive = 15;
};

#endif // OBJECTITEMCUBE_H
