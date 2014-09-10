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


enum E_TRACER_STATE
{
    ETS_DEFAULT,
    ETS_WARNING,
    ETS_PANIC,
    ETS_CRASHING
};

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
        scene::SMeshBuffer* Buffer;
        core::vector3df PlayerPos;
        f32 PlayerSpeed = 1;
        bool Mocking = false;
        E_TRACER_STATE State = ETS_DEFAULT;
        f32 FadeProgress = 0;

        video::SColor DefaultColor, WarnColor, PanicColor;
        static constexpr f32 Advance = 1.5;
        static constexpr f32 WarnDist = 18;
        static constexpr f32 PanicDist = 4;
        static constexpr f32 ColorChangeTime = 2;
        static constexpr f32 Length = 0.7;
        static constexpr f32 Height = 0.04;
        static constexpr u32 Segments = 10;

        static constexpr f32 WarnIn = WarnDist + ColorChangeTime;
        static constexpr f32 PanicIn = PanicDist + ColorChangeTime;
};

#endif // OBJECTTRACER_H
