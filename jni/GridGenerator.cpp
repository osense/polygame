#include "GridGenerator.h"

GridGenerator::GridGenerator(u32 numPoints)
{
    NumPoints = numPoints;
    LastPts = new f32[NumPoints];
    NewPts = new f32[NumPoints];
    ArraySize = 0;
    Type = EGT_NONE;
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

void GridGenerator::genNone()
{
    for (u32 i = 0; i < NumPoints; i++)
    {
        NewPts[i] = LastPts[i];
    }
}

void GridGenerator::genPlains(core::vector3df pos)
{
    NewPts[0] = 0;
    NewPts[NumPoints-1] = 0;

    for (u32 i = 1; i < NumPoints-1; i++)
    {
        NewPts[i] = PerlinN.GetValue(pos.X*0.1 + 0.5 + i, 0, pos.Z*0.1 + 0.5);
        if (NewPts[i] < 0)
            NewPts[i] = 0;
        /*u32 rnd = rand()%100;
        if (rnd > 75)
            NewPts[i] = (LastPts[i-1] + LastPts[i] +LastPts[i+1]) / 3 + (rnd/25.0-3.1);
        else
            NewPts[i] = 0;*/
    }
}

void GridGenerator::genCanyons(core::vector3df pos)
{

}
