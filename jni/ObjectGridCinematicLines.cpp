#include "ObjectGridCinematicLines.h"

ObjectGridCinematicLines::ObjectGridCinematicLines(SContext* cont, u32 numPtsX) : Object(cont),
    NumPointsX(numPtsX)
{
    Name = "ObjectGridCinematicLines";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    LineMesh = GeometryGenerator::createLineMesh(1);
}

ObjectGridCinematicLines::~ObjectGridCinematicLines()
{
    while(LGroups.size() > 0)
    {
    	LGroups[0].Node->remove();
    	LGroups.erase(LGroups.begin());
    }

	delete LineMesh;

    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGridCinematicLines::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (LGroups.size() == 0)
        {
        	return;
        }

        for (u32 i = 0; i < LGroups.size(); i++)
        {
        	LGroups[i].TimeSinceUpdate += msg.Update.fDelta;
        }

        auto thisGroup = &LGroups[LGroupUpdateIdx];
        auto deadGroup = true;
        auto buffer = static_cast<scene::SMeshBuffer*>(thisGroup->Node->getMesh()->getMeshBuffer(0));
        const f32 fadeStep = (thisGroup->TimeSinceUpdate / FadeTime);

        for (u32 i = 0; i < thisGroup->Lines.size(); i++)
        {
        	if (thisGroup->Lines[i].dead)
        	{
        		continue;
        	}
        	deadGroup = false;

        	thisGroup->Lines[i].ttl -= thisGroup->TimeSinceUpdate;

        	const u32 vertIdx = i * 8;
        	for (u32 j = 0; j < 8; j++)
        	{
        		buffer->Vertices[vertIdx + j].Pos += thisGroup->Lines[i].dirV * thisGroup->TimeSinceUpdate;
        	}

        	if (thisGroup->Lines[i].ttl <= FadeTime)
        	{
        		s32 newAlpha = buffer->Vertices[vertIdx].Color.getAlpha() - s32(255 * fadeStep);
        		if (newAlpha < 0)
        		{
        			newAlpha = 0;
        			thisGroup->Lines[i].dead = true;
        		}

        		for (u32 j = 0; j < 8; j++)
        		{
        			buffer->Vertices[vertIdx + j].Color.setAlpha(newAlpha);
        		}
        	}
        }

        if (deadGroup)
        {
        	thisGroup->Node->remove();
        	LGroups.erase(LGroups.begin() + LGroupUpdateIdx);
        }
        else
        {
        	thisGroup->TimeSinceUpdate = 0;
        	LGroupUpdateIdx++;
        }

        if (LGroupUpdateIdx >= LGroups.size())
        {
        	LGroupUpdateIdx = 0;
        }
    }
}

void ObjectGridCinematicLines::spawn(core::vector3df pos, video::SColorf col, video::SColorf farcol, f32* last, f32* prev)
{
	scene::ISceneManager* smgr = Context->Device->getSceneManager();
	scene::SMeshBuffer* lineBuffer = static_cast<scene::SMeshBuffer*>(LineMesh->getMeshBuffer(0));

	LineGroup newLg;
	auto mesh = new scene::SMesh();
	auto mBuffer = new scene::SMeshBuffer();
	mesh->addMeshBuffer(mBuffer);
	mBuffer->drop();
    newLg.Node = smgr->addMeshSceneNode(mesh);
    mesh->drop();
    newLg.Node->setPosition(pos);
    newLg.Node->setMaterialType(Context->Mtls->Alpha);
    newLg.Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    newLg.Node->setAutomaticCulling(scene::EAC_OFF);
    newLg.Node->getMaterial(0).AmbientColor = col.toSColor();
    newLg.Node->getMaterial(0).DiffuseColor = farcol.toSColor();

    f32 halfPtsX = NumPointsX / 2;
    for (u32 i = 1; i < NumPointsX - 1; i++)
    {
    	for (u32 j = 0; j < 4; j++)
    	{
    		lineBuffer->Vertices[j].Pos.X = -halfPtsX + i;
    		lineBuffer->Vertices[j].Pos.Y = prev[i];
    	}
    	for (u32 j = 4; j < 8; j++)
    	{
    		lineBuffer->Vertices[j].Pos = core::vector3df(-halfPtsX + i, last[i], -1);
    	}

    	mBuffer->append(lineBuffer->Vertices.pointer(), lineBuffer->Vertices.size(), lineBuffer->Indices.pointer(), lineBuffer->Indices.size());
    	core::vector3df dir = core::vector3df(0, 0, -MinDirVelocity) + core::vector3df(((rand()%100)/100.0) * DirVelocityOffset);
        core::vector3df rot = core::vector3df(MinRotVelocity) + core::vector3df(((rand()%100)/100.0) * RotVelocityOffset);
        newLg.Lines.push_back(LineGroup::Line(dir, rot, ExistTime + (((rand()%100)/100.0) * ExistTimeOffset)));
    }

    newLg.TimeSinceUpdate = 0;
    LGroups.push_back(newLg);
}

u32 ObjectGridCinematicLines::getLineCount() const
{
    u32 c = 0;
    for (u32 i = 0; i < LGroups.size(); i++)
    {
    	c += LGroups[i].Lines.size();
    }

    return c;
}
