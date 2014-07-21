#ifndef OBJECTITEMCUBE_H
#define OBJECTITEMCUBE_H

#include "Object.h"
#include "ObjectManager.h"
#include "SMaterials.h"
#include <irrlicht.h>

using namespace irr;

class ObjectItemCube : public Object
{
    public:
        ObjectItemCube(SContext* cont, core::vector3df pos);

        virtual ~ObjectItemCube();

        virtual void onMessage(SMessage msg);

        scene::IMesh* getCubeMesh();

    private:
        scene::ISceneNode* Node;

        static const bool Filled = false;
        static const f32 CubeSize = 0.25;
        static const f32 RotSpeed = 0.025;
};

#endif // OBJECTITEMCUBE_H
