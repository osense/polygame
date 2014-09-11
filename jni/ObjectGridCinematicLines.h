#ifndef OBJECTGRIDCINEMATICLINES_H
#define OBJECTGRIDCINEMATICLINES_H

#include "Object.h"
#include "ObjectManager.h"
#include "GeometryGenerator.h"
#include "SMaterials.h"
#include <irrlicht.h>
#include <vector>
#include <stdlib.h>

using namespace irr;


class ObjectGridCinematicLines : public Object
{
    public:
        ObjectGridCinematicLines(SContext* cont, u32 numPtsX);
        virtual ~ObjectGridCinematicLines();

        virtual void onMessage(SMessage msg);

        void spawn(core::vector3df pos, video::SColorf col, video::SColorf farcol, f32* last, f32* prev);

        u32 getLineCount() const;

    private:
        struct LineGroup
        {
        	struct Line
        	{
        		core::vector3df dirV;
        		core::vector3df rotV;
        		f32 ttl;
        		bool dead;

        		Line(core::vector3df d, core::vector3df r, f32 t)
            		:dirV(d), rotV(r), ttl(t), dead(false)
        		{

        		}
        	};

        	std::vector<Line> Lines;
            scene::IMeshSceneNode* Node;
            f32 TimeSinceUpdate;
        };


        u32 NumPointsX;
        std::vector<LineGroup> LGroups;
        u32 LGroupUpdateIdx = 0;
        scene::IMesh* LineMesh;


        static constexpr f32 ExistTime = 3;
        static constexpr f32 ExistTimeOffset = 2;
        static constexpr f32 FadeTime = 1;

        static constexpr f32 MinDirVelocity = 0.75;
        static constexpr f32 DirVelocityOffset = 0.5;
        static constexpr f32 MinRotVelocity = -5;
        static constexpr f32 RotVelocityOffset = 10;
};

#endif // OBJECTGRIDCINEMATICLINES_H
