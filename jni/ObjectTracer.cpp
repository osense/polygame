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

    PlayerPos = core::vector3df(0);
    PositionsIdx = 0;

    scene::IMesh* mesh = GeometryGenerator::createTracerMesh(Length, Height, Segments);
    Buffer = static_cast<scene::SMeshBuffer*>(mesh->getMeshBuffer(0));
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh, 0, -1, Positions[0]);
    mesh->drop();
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

    Context->Device->getSceneManager()->addToDeletionQueue(Node);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectTracer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        f32 nodeZ = PlayerPos.Z + Advance;

        if (Mocking)
        {
            nodeZ = Node->getPosition().Z + msg.Update.fDelta * PlayerSpeed;
            if (nodeZ > PlayerPos.Z + 20)
            {
                delete this;
                return;
            }
        }

        if (Positions[PositionsIdx+2].Z <= nodeZ)
        {
            PositionsIdx++;

            if (PositionsIdx + 3 >= Positions.size())
            {
                if (State != ETS_CRASHING)
                {
                    Positions.push_back(Positions.back() + core::vector3df(0, 0, 1));
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

        core::vector3df targetPos = interpCR(Positions[PositionsIdx], Positions[PositionsIdx+1], Positions[PositionsIdx+2], Positions[PositionsIdx+3], nodeZ);
        Node->setPosition(targetPos);

        f32 backZ = Positions.back().Z;
        if (State == ETS_WARNING && backZ - PanicIn <= nodeZ)
        {
            State = ETS_PANIC;
            FadeProgress = 0;
        }
        else if (State == ETS_DEFAULT && backZ - WarnIn <= nodeZ)
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


        for (u32  i = 1; i <= Segments; i++)
        {
            s32 posIdx = PositionsIdx;
            f32 posZ = nodeZ - ((f32(i)/Segments) * Length);
            while (posZ < Positions[posIdx+1].Z)
            {
                posIdx--;

                if (posIdx < 0)
                {
                    return;
                }
            }

            const u32 vertIdx = i * 2;
            core::vector3df thisPos = interpCR(Positions[posIdx], Positions[posIdx+1], Positions[posIdx+2], Positions[posIdx+3], posZ);
            const core::vector3df modelPos = thisPos - targetPos;
            constexpr f32 HeightHalf = Height / 2;
            Buffer->Vertices[vertIdx].Pos = modelPos + core::vector3df(0, HeightHalf, 0);
            Buffer->Vertices[vertIdx+1].Pos = modelPos + core::vector3df(0, -HeightHalf, 0);
            Buffer->setDirty();
        }
    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        PlayerPos = core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z);
        PlayerSpeed = msg.Position.Speed;
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        Mocking = true;
        PlayerSpeed *= 0.6;
    }
}
