#include "ObjectGridCinematicLines.h"

ObjectGridCinematicLines::ObjectGridCinematicLines(SContext* cont, u32 numPtsX, u32 offsetZ) : Object(cont),
    NumPointsX(numPtsX)
{
    Name = "ObjectGridCinematicLines";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    OffsetZ = -1.0 * offsetZ;

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
}

ObjectGridCinematicLines::~ObjectGridCinematicLines()
{
    while(Lines.size() > 0)
    {
        Lines[0].node->remove();
        Lines.erase(0);
    }

    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGridCinematicLines::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        f32 fadeStep = msg.Update.fDelta / FadeTime;

        for (u32 i = 0; i < Lines.size(); i++)
        {
            Lines[i].node->setPosition(Lines[i].node->getPosition() + Lines[i].dirV * msg.Update.fDelta);
            Lines[i].node->setRotation(Lines[i].node->getRotation() + Lines[i].rotV * msg.Update.fDelta);
            Lines[i].ttl -= msg.Update.fDelta;

            if (Lines[i].ttl <= FadeAfterTime)
            {
                Lines[i].node->getMaterial(0).MaterialTypeParam += fadeStep;

                if (Lines[i].ttl <= 0)
                {
                    Lines[i].node->remove();
                    Lines.erase(i--);
                }
            }
        }
    }
}

void ObjectGridCinematicLines::spawn(core::vector3df pos, video::SColorf col, video::SColorf farcol, f32* last, f32* prev)
{
    u32 halfPtsX = NumPointsX / 2;
    u32 quarterPtsX = halfPtsX / 2;
    video::SColor c = col.toSColor(), fc = farcol.toSColor();

    for (u32 i = 1; i < NumPointsX - 1; i++)
    {
        scene::ISceneManager* smgr = Context->Device->getSceneManager();
        scene::IMeshSceneNode* node = smgr->addMeshSceneNode(smgr->getMesh("line-mesh"));
        node->setMaterialType(Context->Mtls->Alpha);
        node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
        node->getMaterial(0).AmbientColor = c;
        node->getMaterial(0).DiffuseColor = fc;
        node->setPosition(pos + core::vector3df(f32(i) - halfPtsX, last[i], OffsetZ));
        f32 heightDiff = last[i] - prev[i];
        f32 rotAngleRad = atan(heightDiff);
        node->setRotation(core::vector3df(radToDeg(rotAngleRad), 0, 0));
        node->setScale(core::vector3df(1, 1, abs(1 / cos(rotAngleRad))));

        core::vector3df dir = core::vector3df(0, 0, -MinDirVelocity) + core::vector3df(((rand()%100)/100.0) * DirVelocityOffset);
        core::vector3df rot = core::vector3df(MinRotVelocity) + core::vector3df(((rand()%100)/100.0) * RotVelocityOffset);
        Lines.push_back(Line(node, dir, rot, ExistTime + (((rand()%100)/100.0) * ExistTimeOffset)));

        // awesome LOD right here
        if ((i < quarterPtsX) || (i > halfPtsX + quarterPtsX))
        {
            i++;
        }
    }
}

u32 ObjectGridCinematicLines::getLineCount() const
{
    return Lines.size();
}
