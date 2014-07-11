#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include <stdlib.h>
#include <irrlicht.h>

#include "libnoise/perlin.h"
#include <iostream>

using namespace irr;

enum E_GEN_TYPE
{
    EGT_NONE = 0,
    EGT_PLAINS,
    EGT_CANYONS,
    EGT_WALLS
};

class GridGenerator
{
    public:
        GridGenerator(u32 numPoints);
        ~GridGenerator();

        void addPoint(f32 p);
        void setPoints(f32 *p);
        void reset();
        f32* generate(core::vector3df position);
        f32 getGenerated(u32 idx) const;

        void setType(E_GEN_TYPE type);

    private:
        u32 NumPoints;
        f32* LastPts, *NewPts;
        u32 ArraySize;

        E_GEN_TYPE Type;

        noise::Perlin PerlinN;

        void genNone();
        void genPlains(core::vector3df pos);
        void genCanyons(core::vector3df pos);
};

#endif // GRIDGENERATOR_H
