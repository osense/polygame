#include "ObjectTracer.h"

ObjectTracer::ObjectTracer(SContext* cont, Json::Value& jsonData) : Object(cont)
{
    Name = "ObjectTracer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectPlayer")->registerObserver(this);

    io::IReadFile* inF = Context->Device->getFileSystem()->createAndOpenFile(Context->StoragePath + jsonData["filename"].asString().c_str());
    u32 numPositions = jsonData["count"].asUInt();
    Positions.resize(numPositions);
    inF->read(Positions.data(), sizeof(core::vector3df) * numPositions);

    PositionsIdx = 0;

    GeometryGenerator gGen;
    Node = Context->Device->getSceneManager()->addMeshSceneNode(gGen.createTracerMesh(Length, Height, Segments), 0, -1, Positions[0]);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->Solid);
    Node->getMaterial(0).AmbientColor = video::SColor(255, 255, 255, 255);
}

ObjectTracer::~ObjectTracer()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);
    Object* pl = Context->ObjManager->getObjectFromName("ObjectPlayer");
    if (pl)
    {
        pl->unregisterObserver(this);
    }

    Node->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectTracer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        f32 newZ = PlayerZ + Advance;

        if (Positions[PositionsIdx+2].Z <= Node->getPosition().Z)
        {
            PositionsIdx++;
            if (PositionsIdx + 4 >= Positions.size())
            {
                if (!Crashing)
                {
                    Positions.push_back(Positions.back() + core::vector3df(0, 0, 0.5));
                    Crashing = true;
                }
                else
                {
                    delete this;
                    return;
                }
            }
        }

        Node->setPosition(interpCR(Positions[PositionsIdx], Positions[PositionsIdx+1], Positions[PositionsIdx+2], Positions[PositionsIdx+3], newZ));
    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        PlayerZ = msg.Position.Z;
    }
}
