#ifndef OBJECTITEMCUBE_H
#define OBJECTITEMCUBE_H

#include "ObjectItem.h"
#include "ObjectManager.h"
#include "GeometryGenerator.h"
#include "SMaterials.h"
#include "ObjectGrid.h"
#include <irrlicht.h>

using namespace irr;

class ObjectItemCube : public ObjectItem
{
    public:
        ObjectItemCube(SContext* cont, core::vector3df pos);

        virtual ~ObjectItemCube();

        virtual void onMessage(SMessage msg);

        static constexpr f32 getCubeSize()
        {
            return CubeSize;
        }

    private:
        scene::ISceneNode* Node;
        f32 EffectCounter;

        static constexpr bool Filled = false;
        static constexpr f32 CubeSize = 0.25;
        static constexpr f32 RotSpeed = 40;
        static constexpr f32 DestroyDist = 2;
        static constexpr f32 TimeActive = 15;
        static constexpr f32 TimeFadein = 0.75;
};

#endif // OBJECTITEMCUBE_H
