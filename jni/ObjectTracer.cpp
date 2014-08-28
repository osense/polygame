#include "ObjectTracer.h"

ObjectTracer::ObjectTracer(SContext* cont, Json::Value& jsonData) : Object(cont)
{
    Name = "ObjectTracer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    DefaultColor = video::SColor(255, 255, 255, 255);
    WarnColor = video::SColor(255, 200, 255, 0);
    PanicColor = video::SColor(255, 255, 30, 0);

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
    Node->setMaterialType(Context->Mtls->Tracer);
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

            if (PositionsIdx + 3 >= Positions.size())
            {
                if (State != ETS_CRASHING)
                {
                    Positions.push_back(Positions.back() + core::vector3df(0, 0, 1));
                    State = ETS_CRASHING;
                }
                else
                {
                    delete this;
                    return;
                }
            }
        }

        f32 lastZ = Positions.back().Z;

        if (State == ETS_WARNING && lastZ - PanicIn <= newZ)
        {
            State = ETS_PANIC;
            FadeProgress = 0;
        }
        else if (State == ETS_DEFAULT && lastZ - WarnIn <= newZ)
        {
            State = ETS_WARNING;
            FadeProgress = 0;
        }

        if (State != ETS_DEFAULT)
        {
            video::SColor targetCol, baseCol;
            if (State == ETS_WARNING)
            {
                targetCol = WarnColor;
                baseCol = DefaultColor;
            }
            else
            {
                targetCol = PanicColor;
                baseCol = WarnColor;
            }

            FadeProgress += msg.Update.fDelta;
            Node->getMaterial(0).AmbientColor = targetCol.getInterpolated(baseCol, FadeProgress / ColorChangeTime);
        }

        Node->setPosition(interpCR(Positions[PositionsIdx], Positions[PositionsIdx+1], Positions[PositionsIdx+2], Positions[PositionsIdx+3], newZ));
    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        PlayerZ = msg.Position.Z;
    }
}
