#include "ObjectTracer.h"

ObjectTracer::ObjectTracer(SContext* cont, Json::Value& jsonData) : Object(cont)
{
    Name = "ObjectTracer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    io::IReadFile* inF = Context->Device->getFileSystem()->createAndOpenFile(Context->StoragePath + jsonData["filename"].asString().c_str());
    u32 numPositions = jsonData["count"].asUInt();
    Positions.resize(numPositions);
    inF->read(Positions.data(), sizeof(core::vector3df) * numPositions);
}

ObjectTracer::~ObjectTracer()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectTracer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {

    }
}
