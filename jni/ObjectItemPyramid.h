#ifndef OBJECTITEMPYRAMID_H
#define OBJECTITEMPYRAMID_H

#include "ObjectItem.h"
#include "ObjectManager.h"
#include "SMaterials.h"


class ObjectItemPyramid : public ObjectItem
{
    public:
        ObjectItemPyramid(SContext* cont, core::vector3df pos, bool negative);
        ~ObjectItemPyramid();

        virtual void onMessage(SMessage msg);

        static constexpr f32 getPyramidSize()
        {
            return Size;
        }

    private:
        scene::ISceneNode* Node;
        f32 EffectCounter = 0;
        s32 Negativity = 1;

        static constexpr f32 Size = 0.3;
        static constexpr f32 RotSpeed = 60;
        static constexpr f32 DestroyDist = 2;
        static constexpr f32 TimeActive = 15;
        static constexpr f32 TimeFadein = 1.5;
};

#endif // OBJECTITEMPYRAMID_H
