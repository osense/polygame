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

    private:
        scene::ISceneNode* Node;

        scene::IMesh* getCubeMesh() const;

        static const f32 Size = 0.3;
};

#endif // OBJECTITEMCUBE_H
