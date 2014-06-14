#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont)
{
    Name = "ObjectGrid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));


    Translation.makeIdentity();
    Translation.setTranslation(core::vector3df(0));

    for (u32 i = 0; i < NumPoints; i++)
        for (u32 j = 0; j < NumPoints; j++)
            Points[i][j] = 0;

    Points[10][15] = 2;
    Points[11][15] = 1;
    Points[11][14] = 0.4;
    Points[12][15] = 1;

    Buffer = new scene::SMeshBuffer();
    scene::SMesh* mesh = new scene::SMesh();
    mesh->setHardwareMappingHint(scene::EHM_DYNAMIC);
    mesh->addMeshBuffer(Buffer);
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setAutomaticCulling(scene::EAC_OFF);

    //LineBuffer = static_cast<scene::SMeshBuffer*>(Context->Device->getSceneManager()->getMesh("line.obj")->getMesh(0)->getMeshBuffer(0));

    regenerate();
}

ObjectGrid::~ObjectGrid()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGrid::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        core::vector3df center(0);

    }
}

void ObjectGrid::regenerate()
{
    #ifdef DEBUG_GRID
    u32 genStart = Context->Device->getTimer()->getTime();
    #endif // DEBUG_GRID

    Buffer->Vertices.clear();
    Buffer->Indices.clear();

    core::vector3df center(NumPoints/2.0, 0, NumPoints/2.0);

    for (u32 i = 1; i < NumPoints; i++)
    {
        for (u32 j = 1; j < NumPoints; j++)
        {
            core::vector3df l1start, l2start, l1end, l2end;
            l1start = l2start = l1end = l2end = -center;

            l1start += core::vector3df(i-1, Points[i-1][j], j);
            l2start += core::vector3df(i, Points[i][j-1], j-1);
            l1end += core::vector3df(i, Points[i][j], j);
            l2end += core::vector3df(i, Points[i][j], j);

            f32 thicknessCorrection = ((i-NumPoints/2.0)/NumPoints) * ((j-NumPoints/2.0)/NumPoints);//  //((i*j - ((NumPoints*NumPoints)/2.0)) / (NumPoints*NumPoints));
            thicknessCorrection *= 0.1;
            if (thicknessCorrection < 0) thicknessCorrection *= -1;
            thicknessCorrection += LineThickness;

            core::vector3df distModX(thicknessCorrection, 0, 0);
            core::vector3df distModY(0, thicknessCorrection, 0);
            core::vector3df distModZ(0, 0, thicknessCorrection);

            // line 1
            u32 indS = Buffer->Vertices.size();
            Buffer->Vertices.push_back(video::S3DVertex(l1start - distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1start + distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1end - distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 1.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1end + distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 1.0)));

            Buffer->Vertices.push_back(video::S3DVertex(l1start - distModZ, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1start + distModZ, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1end - distModZ, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 1.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l1end + distModZ, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 1.0)));

            Buffer->Indices.push_back(indS); Buffer->Indices.push_back(indS+1); Buffer->Indices.push_back(indS+2);
            Buffer->Indices.push_back(indS+1); Buffer->Indices.push_back(indS+3); Buffer->Indices.push_back(indS+2);

            Buffer->Indices.push_back(indS+4); Buffer->Indices.push_back(indS+5); Buffer->Indices.push_back(indS+6);
            Buffer->Indices.push_back(indS+5); Buffer->Indices.push_back(indS+7); Buffer->Indices.push_back(indS+6);

            // line 2
            indS = Buffer->Vertices.size();
            Buffer->Vertices.push_back(video::S3DVertex(l2start - distModX, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2start + distModX, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2end - distModX, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 1.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2end + distModX, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 1.0)));

            Buffer->Vertices.push_back(video::S3DVertex(l2start - distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2start + distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 0.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2end - distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(0.0, 1.0)));
            Buffer->Vertices.push_back(video::S3DVertex(l2end + distModY, irr::core::vector3df(0), irr::video::SColor(255,255,255,255), irr::core::vector2df(1.0, 1.0)));

            Buffer->Indices.push_back(indS); Buffer->Indices.push_back(indS+1); Buffer->Indices.push_back(indS+2);
            Buffer->Indices.push_back(indS+1); Buffer->Indices.push_back(indS+3); Buffer->Indices.push_back(indS+2);

            Buffer->Indices.push_back(indS+4); Buffer->Indices.push_back(indS+5); Buffer->Indices.push_back(indS+6);
            Buffer->Indices.push_back(indS+5); Buffer->Indices.push_back(indS+7); Buffer->Indices.push_back(indS+6);
        }
    }

    Buffer->setDirty();
    Buffer->recalculateBoundingBox();

    #ifdef DEBUG_GRID
    u32 genEnd = Context->Device->getTimer()->getTime();
    debugLog(core::stringc("Regenerated grid: ") + core::stringc(genStart - genEnd));
    #endif // DEBUG_GRID
}
