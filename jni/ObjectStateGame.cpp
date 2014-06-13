#include "ObjectStateGame.h"

ObjectStateGame::ObjectStateGame(SContext* cont) : Object(cont)
{
    Name = "ObjectStateGame";
    setPersistent(true);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));
}

ObjectStateGame::~ObjectStateGame()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateGame::onMessage(SMessage msg)
{

}
