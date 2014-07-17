#ifndef OBJECTITEMSPAWNER_H
#define OBJECTITEMSPAWNER_H

#include "Object.h"
#include "ObjectGrid.h"

#include "libnoise/perlin.h"


class ObjectItemSpawner : public Object
{
    public:
        ObjectItemSpawner(SContext* cont);
        ~ObjectItemSpawner();

        virtual void onMessage(SMessage msg);

    private:
        ObjectGrid* Grid;
        noise::Perlin PerlinN;

        u32 SpawnMinX;
        u32 SpawnMaxX;

        static const f32 SpawnChance = 0.05;
        static const f32 SpawnHillLimit = 0.2;
        static const f32 SpawnItemHeight = 0.5;
};

#endif // OBJECTITEMSPAWNER_H
