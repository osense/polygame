#ifndef OBJECTITEMSPAWNER_H
#define OBJECTITEMSPAWNER_H

#include "Object.h"
#include "ObjectGrid.h"
#include "functions.h"
#include "json/value.h"
#include "libnoise/perlin.h"

#include "ObjectItemCube.h"
#include "ObjectItemPyramid.h"
#include "ObjectTracer.h"


class ObjectItemSpawner : public Object
{
    public:
        ObjectItemSpawner(SContext* cont);
        ~ObjectItemSpawner();

        virtual void onMessage(SMessage msg);

        void spawnTracers() const;

    private:
        ObjectGrid* Grid;
        noise::Perlin PerlinN;

        u32 SpawnMinX;
        u32 SpawnMaxX;

        const f32 SpawnChance = 0.005;
        const f32 SpawnHillLimit = 0.2;
        const f32 SpawnItemHeight = 0.4;
};

#endif // OBJECTITEMSPAWNER_H
