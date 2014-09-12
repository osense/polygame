#include "ObjectGridCinematicLines.h"

ObjectGridCinematicLines::ObjectGridCinematicLines(SContext* cont, u32 numPtsX) : Object(cont),
    NumPointsX(numPtsX)
{
    Name = "ObjectGridCinematicLines";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdaterThreaded")->registerObserver(this);

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

    Context->ObjManager->getObjectFromName("ObjectUpdaterThreaded")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGridCinematicLines::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
    	for (std::vector<LineGroup>::iterator it = LGroups.begin(); it != LGroups.end(); it++)
    	{
    		updateLineGroup(*it, msg.Update.fDelta);
    	}
    }
}

void ObjectGridCinematicLines::spawn(core::vector3df pos, video::SColorf col, video::SColorf farcol, f32* last, f32* prev)
{
	cleanDeadGroups();

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

    LGroups.push_back(newLg);
}

void ObjectGridCinematicLines::updateLineGroup(LineGroup& group, f32 fDelta)
{
	group.Dead = true;
	auto buffer = static_cast<scene::SMeshBuffer*>(group.Node->getMesh()->getMeshBuffer(0));
	const f32 fadeStep = fDelta / FadeTime;

	for (u32 i = 0; i < group.Lines.size(); i++)
	{
		if (group.Lines[i].dead)
		{
			continue;
		}
		group.Dead = false;

		group.Lines[i].ttl -= fDelta;

		const u32 vertIdx = i * 8;
		for (u32 j = 0; j < 8; j++)
		{
			buffer->Vertices[vertIdx + j].Pos += group.Lines[i].dirV * fDelta;
		}

		if (group.Lines[i].ttl <= FadeTime)
		{
			s32 newAlpha = buffer->Vertices[vertIdx].Color.getAlpha() - s32(255 * fadeStep);
			if (newAlpha < 0)
			{
				newAlpha = 0;
				group.Lines[i].dead = true;
			}

			for (u32 j = 0; j < 8; j++)
			{
				buffer->Vertices[vertIdx + j].Color.setAlpha(newAlpha);
			}
		}
	}

	if (group.Dead)
	{
		group.Node->setVisible(false);
	}
}

void ObjectGridCinematicLines::cleanDeadGroups()
{
	for (auto it = LGroups.begin(); it != LGroups.end(); it++)
	{
		if (it->Dead)
		{
			it->Node->remove();
			LGroups.erase(it);

			it = LGroups.begin();
		}
	}
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

u32 ObjectGridCinematicLines::getGroupCount() const
{
	return LGroups.size();
}
