#ifndef GRIDGENERATOR_H
#define GRIDGENERATOR_H

#include <irrlicht.h>
#include "functions.h"
#include "circular_buffer.h"
#include "libnoise/perlin.h"
#include "json/value.h"

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

enum E_GEN_DIRECTION
{
    EGD_FRONT,
    EGD_LEFT,
    EGD_RIGHT
};

enum E_SLOPE_TYPE
{
    EST_NONE = 0,
    EST_DOWN,
    EST_UP,
    EST_COUNT
};

class GridGenerator
{
    public:
        GridGenerator(u32 seed, u32 numPointsX, u32 numPointsZ);
        ~GridGenerator();

        f32* generate(core::vector3df position, E_GEN_DIRECTION dir);
        f32 getGenerated(u32 idx) const;

        void setType(E_GEN_TYPE type);
        E_GEN_TYPE getType() const;

        void setSlope(E_SLOPE_TYPE type);
        E_SLOPE_TYPE getSlope() const;
        u32 getSlopeChangeInSteps() const;
        f32 getHeight(u32 z) const;

        void setDifficulty(f32 diff);
        f32 getDifficulty() const;

        f32 getRandomVal(f32 posZ) const;

        Json::Value serialize() const;
        void deserialize(Json::Value& root);

    private:
        f32 Difficulty;

        u32 NumPointsX, NumPointsZ;
        f32* NewPts;
        u32 ArraySize;

        E_GEN_TYPE Type;
        E_SLOPE_TYPE Slope, PrevSlope, NextSlope;
        circular_buffer<f32> Height;
        u32 StepsIntoSlope = 0;

        noise::Perlin PerlinN;

        f32 genNone(core::vector3df pos);
        f32 genPlains(core::vector3df pos);
        f32 genHills(core::vector3df pos);
        f32 genCanyons(core::vector3df pos);
        f32 genWalls(core::vector3df pos);

        void slopeTransform();

        const f32 SlopeStep = 0.33;
        const u32 SlopeChangeInSteps = 6;
};

#endif // GRIDGENERATOR_H
