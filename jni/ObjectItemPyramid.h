#ifndef OBJECTITEMPYRAMID_H
#define OBJECTITEMPYRAMID_H

#include "ObjectItem.h"
#include "ObjectManager.h"
#include "GeometryGenerator.h"
#include "SMaterials.h"
#include "ShaderCBGrid.h"
#include "ShaderCBGridBack.h"
#include "ShaderCBCube.h"


class ObjectItemPyramid : public ObjectItem
{
    public:
        ObjectItemPyramid(SContext* cont, core::vector3df pos);
        ~ObjectItemPyramid();

        virtual void onMessage(SMessage msg);

        scene::IMesh* getPyramidMesh();

    private:
        scene::ISceneNode* Node;
        f32 EffectCounter;

        static const bool Filled = false;
        static const f32 Size = 0.3;
        static const f32 RotSpeed = 0.025;
        static const f32 DestroyDist = 2;
        static const f32 TimeActive = 15;
};

#endif // OBJECTITEMPYRAMID_H
