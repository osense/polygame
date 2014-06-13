#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont),
    Pool(Context)
{
    Name = "ObjectGrid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Pool.prepare(100);
}

ObjectGrid::~ObjectGrid()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGrid::onMessage(SMessage msg)
{

}
