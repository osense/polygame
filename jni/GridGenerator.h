#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include <stdlib.h>
#include <irrlicht.h>
#include "json/value.h"

#include "libnoise/perlin.h"

using namespace irr;

enum E_GEN_TYPE
{
    EGT_NONE = 0xffff,
    EGT_PLAINS = 0,
    EGT_HILLS,
    EGT_CANYONS,
    EGT_WALLS,
    EGT_COUNT
};

enum E_SLOPE_TYPE
{
    EST_NONE = 0,
    EST_DOWN,
    EST_UP
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
        E_GEN_TYPE getType() const;

        void setSlope(E_SLOPE_TYPE type);
        f32 getHeight() const;

        void setDifficulty(f32 diff);
        f32 getDifficulty() const;

        Json::Value serialize() const;
        void deserialize(Json::Value& root);

    private:
        f32 Difficulty;

        u32 NumPoints;
        f32* LastPts, *NewPts;
        u32 ArraySize;

        E_GEN_TYPE Type;
        E_SLOPE_TYPE Slope, PrevSlope, NextSlope;
        f32 Height;
        u32 StepsIntoSlope;

        noise::Perlin PerlinN;

        void genNone();
        void genPlains(core::vector3df pos);
        void genHills(core::vector3df pos);
        void genCanyons(core::vector3df pos);
        void genWalls(core::vector3df pos);

        void slopeTransform();

        const f32 SlopeStep = 0.33;
        const u32 SlopeChangeInSteps = 6;
};

#endif // GRIDGENERATOR_H
