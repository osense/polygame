#include "GridGenerator.h"

GridGenerator::GridGenerator(u32 numPointsX, u32 numPointsZ)
    :Height(numPointsZ)
{
    NumPointsX = numPointsX;
    NumPointsZ = numPointsZ;
    NewPts = new f32[NumPointsX];
    ArraySize = 0;
    Type = EGT_NONE;
    Slope = EST_NONE;
    NextSlope = EST_NONE;
    Difficulty = 0;

    for (u32 z = 0; z < NumPointsZ; z++)
        Height.push_back(0);

    //PerlinN.SetNoiseQuality(noise::QUALITY_BEST);
    PerlinN.SetSeed(0);
    PerlinN.SetOctaveCount(1);
}

GridGenerator::~GridGenerator()
{
    delete[] NewPts;
}

f32* GridGenerator::generate(core::vector3df position, E_GEN_DIRECTION dir)
{
    f32 (GridGenerator::*genFunc)(core::vector3df);

    switch(Type)
    {
    case EGT_PLAINS:
        genFunc = &GridGenerator::genPlains;
        break;
    case EGT_HILLS:
        genFunc = &GridGenerator::genHills;
        break;
    case EGT_CANYONS:
        genFunc = &GridGenerator::genCanyons;
        break;
    case EGT_WALLS:
        genFunc = &GridGenerator::genWalls;
        break;

    case EGT_NONE:
    default:
        genFunc = &GridGenerator::genNone;
        break;
    }

    switch (dir)
    {
    case EGD_FRONT:
        slopeTransform();

        for (u32 i = 0; i < NumPointsX; i++)
        {
            NewPts[i] = getHeight(NumPointsZ - 1) + (this->*genFunc)(core::vector3df(position.X + i, 0, position.Z));
        }
        break;

    case EGD_LEFT:
    case EGD_RIGHT:
        u32 rightOffset = dir == EGD_RIGHT ? NumPointsX - 1 : 0;

        for (u32 i = 0; i < NumPointsZ; i++)
        {
            NewPts[i] = getHeight(i) + (this->*genFunc)(core::vector3df(position.X + rightOffset, 0, position.Z + i));
        }
        break;
    }

    return NewPts;
}

f32 GridGenerator::getGenerated(u32 idx) const
{
    return NewPts[idx];
}

void GridGenerator::setType(E_GEN_TYPE type)
{
    Type = type;

    switch (Type)
    {
    case EGT_PLAINS:
        PerlinN.SetFrequency(0.4);
        break;
    case EGT_HILLS:
        PerlinN.SetFrequency(0.25);
        break;
    case EGT_CANYONS:
    case EGT_WALLS:
        PerlinN.SetFrequency(0.4);
        break;
    default:
        break;
    }
}

E_GEN_TYPE GridGenerator::getType() const
{
    return Type;
}

void GridGenerator::setSlope(E_SLOPE_TYPE type)
{
    PrevSlope = Slope;
    StepsIntoSlope = 0;

    if (Slope == EST_UP  && type == EST_DOWN)
    {
        NextSlope = EST_DOWN;
        Slope = EST_NONE;
    }
    else if (Slope == EST_DOWN && type == EST_UP)
    {
        NextSlope = EST_UP;
        Slope = EST_NONE;
    }
    else
    {
        NextSlope = EST_NONE;
        Slope = type;
    }
}

f32 GridGenerator::getHeight(u32 z) const
{
    return Height[Height.getIndex() + z + 1];
}

void GridGenerator::setDifficulty(f32 diff)
{
    Difficulty = diff;
}

f32 GridGenerator::getDifficulty() const
{
    return Difficulty;
}

Json::Value GridGenerator::serialize() const
{
    Json::Value root;
    root["type"] = (u32)Type;
    root["slope"] = (u32)Slope;
    root["slope_prev"] = (u32)PrevSlope;
    root["slope_next"] = (u32)NextSlope;
    root["slope_stepsinto"] = StepsIntoSlope;
    root["difficulty"] = Difficulty;
    root["height"] = serializeCircularBuffer(Height);

    return root;
}

void GridGenerator::deserialize(Json::Value& root)
{
    Type = (E_GEN_TYPE)root["type"].asUInt();
    Slope = (E_SLOPE_TYPE)root["slope"].asUInt();
    PrevSlope = (E_SLOPE_TYPE)root["slope_prev"].asUInt();
    NextSlope = (E_SLOPE_TYPE)root["slope_next"].asUInt();
    StepsIntoSlope = root["slope_stepsinto"].asDouble();
    Difficulty = root["difficulty"].asDouble();
    deserializeCircularBuffer(Height, root["height"]);
}


f32 GridGenerator::genNone(core::vector3df pos)
{
    return 0;
}

f32 GridGenerator::genPlains(core::vector3df pos)
{
    f32 newPt = PerlinN.GetValue(pos.X, 0.5, pos.Z);
    if (newPt < 0.5 - Difficulty/2)
        newPt = 0;

    return newPt;
}

f32 GridGenerator::genHills(core::vector3df pos)
{
    f32 newPt = PerlinN.GetValue(pos.X, 0.5, pos.Z) * 2;
    if (newPt < 1 - Difficulty)
        newPt = 0;

    return newPt;
}


f32 GridGenerator::genCanyons(core::vector3df pos)
{
    f32 newPt = PerlinN.GetValue(pos.X, 0.5, pos.Z * 0.5);
    if (newPt < 0)
        newPt = 0;

    return newPt;
}


f32 GridGenerator::genWalls(core::vector3df pos)
{
    f32 newPt = PerlinN.GetValue(pos.X * 0.5, 0.5, pos.Z);
    if (newPt < 0)
        newPt = 0;

    return newPt;
}

void GridGenerator::slopeTransform()
{
    if (NextSlope != EST_NONE)
        if (StepsIntoSlope >= SlopeChangeInSteps)
            setSlope(NextSlope);

    StepsIntoSlope++;
    f32 nextHeight = getHeight(NumPointsZ - 1);

    if (Slope == EST_NONE)
    {
        if (StepsIntoSlope < SlopeChangeInSteps)
        {
            if (PrevSlope == EST_DOWN)
                nextHeight -= SlopeStep * (1 - (StepsIntoSlope / f32(SlopeChangeInSteps)));
            else if (PrevSlope == EST_UP)
                nextHeight += SlopeStep * (1 - (StepsIntoSlope / f32(SlopeChangeInSteps)));
        }
    }
    else if (Slope == EST_DOWN)
    {
        if (StepsIntoSlope < SlopeChangeInSteps)
            nextHeight -= SlopeStep * (StepsIntoSlope / f32(SlopeChangeInSteps));
        else nextHeight -= SlopeStep;
    }
    else if (Slope == EST_UP)
    {
        if (StepsIntoSlope < SlopeChangeInSteps)
            nextHeight += SlopeStep * (StepsIntoSlope / f32(SlopeChangeInSteps));
        else nextHeight += SlopeStep;
    }

    Height.push_back(nextHeight);
}
