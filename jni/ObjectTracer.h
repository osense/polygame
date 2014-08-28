#ifndef OBJECTTRACER_H
#define OBJECTTRACER_H

#include <irrlicht.h>
#include <vector>
#include "json/value.h"
#include "Object.h"
#include "ObjectManager.h"
#include "SMaterials.h"
#include "GeometryGenerator.h"

using namespace irr;


class ObjectTracer : public Object
{
    public:
        ObjectTracer(SContext* cont, Json::Value& jsonData);
        virtual ~ObjectTracer();

        virtual void onMessage(SMessage msg);

    private:
        std::vector<core::vector3df> Positions;
        u32 PositionsIdx;
        scene::IMeshSceneNode* Node;
        f32 PlayerZ = 0;
        bool Crashing = false;

        static constexpr f32 Advance = 1.5;
        static constexpr f32 Length = 0.75;
        static constexpr f32 Height = 0.015;
        static constexpr u32 Segments = 10;
};

#endif // OBJECTTRACER_H
