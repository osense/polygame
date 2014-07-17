#include "ObjectItemSpawner.h"

ObjectItemSpawner::ObjectItemSpawner(SContext* cont) : Object(cont)
{
    Name = "ObjectItemSpawner";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Grid = static_cast<ObjectGrid*>(Context->ObjManager->getObjectFromName("ObjectGrid"));
    if (!Grid)
    {
        debugLog("Fatal error: no ObjectGrid found to attach to");
        return;
    }

    Grid->registerObserver(this);

    PerlinN.SetOctaveCount(1);
    //PerlinN.SetFrequency(4);

    SpawnMinX = Grid->getNumPointsX() / 4;
    SpawnMaxX = (Grid->getNumPointsX() * 3) / 4;
}

ObjectItemSpawner::~ObjectItemSpawner()
{
    Grid = static_cast<ObjectGrid*>(Context->ObjManager->getObjectFromName("ObjectGrid"));
    if (Grid)
        Grid->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectItemSpawner::onMessage(SMessage msg)
{
    if (msg.Type == EMT_GRID_REGENED)
    {
        core::vector3df gridPos = Grid->getPosition();
        u32 coordY = Grid->getNumPointsY() - 1;

        // iterate the possible positions for spawning
        for (u32 x = SpawnMinX; x < SpawnMaxX; x++)
        {
            // decide where to spawn with the help of Mr. Perlin
            float pVal = abs_(PerlinN.GetValue(x, 0.5, gridPos.Z));
            if (pVal > SpawnChance)
                continue;

            debugLog(core::stringc(pVal));

            // perlin checks out, but what about the terrain?
            if (Grid->getHillHeight(x, coordY) > SpawnHillLimit ||
                Grid->getHillHeight(x + 1, coordY) > SpawnHillLimit ||
                Grid->getHillHeight(x, coordY - 1) > SpawnHillLimit ||
                Grid->getHillHeight(x + 1, coordY - 1) > SpawnHillLimit)
                continue;

            core::vector3df itemPos(gridPos);
            itemPos += core::vector3df(x + 0.5, 0, Grid->getNumPointsY() - 2.5);
            itemPos.X -= Grid->getNumPointsX() / 2.0;
            itemPos.Y = (Grid->getBaseHeight(coordY) + Grid->getBaseHeight(coordY - 1)) / 2.0 + SpawnItemHeight;

            Context->Device->getSceneManager()->addCubeSceneNode(0.3, 0, -1, itemPos);
        }
    }
}
