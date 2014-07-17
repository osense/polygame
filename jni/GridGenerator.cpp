#include "GridGenerator.h"

GridGenerator::GridGenerator(u32 numPoints)
{
    NumPoints = numPoints;
    LastPts = new f32[NumPoints];
    NewPts = new f32[NumPoints];
    ArraySize = 0;
    Type = EGT_NONE;
    Slope = EST_NONE;
    NextSlope = EST_NONE;
    Height = 0;
    Difficulty = 0;

    PerlinN.SetSeed(0);
    PerlinN.SetOctaveCount(1);
}

GridGenerator::~GridGenerator()
{
    //dtor
}

void GridGenerator::addPoint(f32 p)
{
    LastPts[ArraySize] = p;
    ArraySize++;
}

void GridGenerator::setPoints(f32 *p)
{
    memcpy(LastPts, p, sizeof(f32) * NumPoints);
    ArraySize = NumPoints;
}

void GridGenerator::reset()
{
    ArraySize = 0;
}

f32* GridGenerator::generate(core::vector3df position)
{
    switch(Type)
    {
    case EGT_NONE:
        genNone();
        break;
    case EGT_PLAINS:
        genPlains(position);
        break;
    case EGT_CANYONS:
        genCanyons(position);
        break;
    }

    slopeTransform();

    return NewPts;
}

f32 GridGenerator::getGenerated(u32 idx) const
{
    return NewPts[idx];
}

void GridGenerator::setType(E_GEN_TYPE type)
{
    Type = type;
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

f32 GridGenerator::getHeight() const
{
    return Height;
}

void GridGenerator::setDifficulty(f32 diff)
{
    Difficulty = diff;
}

f32 GridGenerator::getDifficulty() const
{
    return Difficulty;
}


void GridGenerator::genNone()
{
    for (u32 i = 0; i < NumPoints; i++)
    {
        NewPts[i] = 0;
    }
}

void GridGenerator::genPlains(core::vector3df pos)
{
    NewPts[0] = 0;
    NewPts[NumPoints-1] = 0;

    PerlinN.SetFrequency(0.4);

    for (u32 i = 1; i < NumPoints-1; i++)
    {
        NewPts[i] = PerlinN.GetValue(pos.X + i, 0.5, int(pos.Z)%150);
        if (NewPts[i] < 0)
            NewPts[i] = 0;
    }
}

void GridGenerator::genCanyons(core::vector3df pos)
{

}

void GridGenerator::slopeTransform()
{
    if (NextSlope != EST_NONE)
        if (StepsIntoSlope >= SlopeChangeInSteps)
            setSlope(NextSlope);

    f32 slpStep = SlopeStep;

    if (Slope == EST_NONE)
    {
        if (StepsIntoSlope <= SlopeChangeInSteps)
        {
            if (PrevSlope == EST_DOWN)
                Height -= slpStep * (1.0 / (1.5 + StepsIntoSlope));
            else if (PrevSlope == EST_UP)
                Height += slpStep * (1.0 / (1.5 + StepsIntoSlope));
        }
    }
    else if (Slope == EST_DOWN)
    {
        if (StepsIntoSlope <= SlopeChangeInSteps)
            Height -= slpStep * (1.0 / (1.5 + SlopeChangeInSteps - StepsIntoSlope));
        else Height -= slpStep;
    }
    else if (Slope == EST_UP)
    {
        if (StepsIntoSlope <= SlopeChangeInSteps)
            Height += slpStep * (1.0 / (1.5 + SlopeChangeInSteps - StepsIntoSlope));
        else Height += slpStep;
    }

    StepsIntoSlope++;

    for (u32 i = 0; i < NumPoints; i++)
    {
        NewPts[i] += Height;
    }
}
