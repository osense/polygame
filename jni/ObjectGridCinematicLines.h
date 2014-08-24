#ifndef OBJECTGRIDCINEMATICLINES_H
#define OBJECTGRIDCINEMATICLINES_H

#include "Object.h"
#include "ObjectManager.h"
#include "SMaterials.h"
#include <irrlicht.h>
#include <stdlib.h>

using namespace irr;


class ObjectGridCinematicLines : public Object
{
    public:
        ObjectGridCinematicLines(SContext* cont, u32 numPtsX, u32 offsetZ);
        virtual ~ObjectGridCinematicLines();

        virtual void onMessage(SMessage msg);

        void spawn(core::vector3df pos, video::SColorf col, video::SColorf farcol, f32* last, f32* prev);

        u32 getLineCount() const;

    private:
        struct Line
        {
            scene::IMeshSceneNode* node;
            core::vector3df dirV;
            core::vector3df rotV;
            f32 ttl;

            Line(scene::IMeshSceneNode* n, core::vector3df d, core::vector3df r, f32 t)
                :node(n), dirV(d), rotV(r), ttl(t)
            {

            }
        };

        u32 NumPointsX;
        f32 OffsetZ;
        core::array<Line> Lines;

        static constexpr f32 ExistTime = 3;
        static constexpr f32 ExistTimeOffset = 2;
        static constexpr f32 FadeTime = 1;
        static constexpr f32 FadeAfterTime = ExistTime - FadeTime;

        static constexpr f32 MinDirVelocity = 0.75;
        static constexpr f32 DirVelocityOffset = 0.5;
        static constexpr f32 MinRotVelocity = -5;
        static constexpr f32 RotVelocityOffset = 10;
};

#endif // OBJECTGRIDCINEMATICLINES_H
