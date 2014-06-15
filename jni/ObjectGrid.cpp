#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont)
{
    Name = "ObjectGrid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Position = core::vector3df(0);

    for (u32 i = 0; i < NumPoints; i++)
        for (u32 j = 0; j < NumPoints; j++)
            Points[i][j] = 0;

    Points[10][15] = 2;
    Points[11][15] = 1;
    Points[11][14] = 0.4;
    Points[12][15] = 1;
    Points[2][20] = 2;

    Buffer = new scene::SMeshBuffer();
    scene::SMesh* mesh = new scene::SMesh();
    mesh->setHardwareMappingHint(scene::EHM_DYNAMIC);
    mesh->addMeshBuffer(Buffer);
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setAutomaticCulling(scene::EAC_OFF);

    regenerate();
}

ObjectGrid::~ObjectGrid()
{
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectGrid::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            msg.Dispatcher->registerObserver(this);
    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        core::vector3df pPos(msg.Position.X, 0, msg.Position.Z);
        core::vector3df diffVect = pPos - Position;

        if (diffVect.getLength() > 1.0)
        {
            #ifdef DEBUG_GRID
            u32 updStart = Context->Device->getTimer()->getTime();
            #endif // DEBUG_GRID

            if (diffVect.Z > 0.5)
                addPlusX();
            else if (diffVect.Z < -0.5)
                addMinusX();

            if (diffVect.X > 0.5)
                addPlusY();
            else if (diffVect.X < -0.5)
                addMinusY();

            #ifdef DEBUG_GRID
            u32 updEnd = Context->Device->getTimer()->getTime();
            debugLog(core::stringc("Updated grid: ") + core::stringc(updStart - updEnd) + "ms");
            #endif // DEBUG_GRID
        }
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
    center -= Position;
    center.Y = 0;

    for (u32 x = 1; x < NumPoints; x++)
    {
        for (u32 y = 1; y < NumPoints; y++)
        {
            core::vector3df l1start, l2start, l1end, l2end;
            l1start = l2start = l1end = l2end = -center;

            l1start += core::vector3df(x-1, Points[x-1][y], y);
            l2start += core::vector3df(x, Points[x][y-1], y-1);
            l1end += core::vector3df(x, Points[x][y], y);
            l2end += core::vector3df(x, Points[x][y], y);

            f32 thicknessCorrection = ((x-NumPoints/2.0)/NumPoints) * ((y-NumPoints/2.0)/NumPoints);//  //((i*j - ((NumPoints*NumPoints)/2.0)) / (NumPoints*NumPoints));
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
    debugLog(core::stringc("Regenerated grid: ") + core::stringc(genStart - genEnd) + "ms");
    #endif // DEBUG_GRID
}


void ObjectGrid::addPlusX()
{
    for (u32 x = 0; x < NumPoints; x++)
        memmove(Points[x], &Points[x][1], sizeof(f32) * (NumPoints-1));

    // init new points
    for (u32 x = 0; x < NumPoints; x++)
        Points[x][NumPoints-1] = 1;

    Position += core::vector3df(0, 0, 1);
    regenerate();
}

void ObjectGrid::addMinusX()
{

}

void ObjectGrid::addPlusY()
{
    memmove(Points, Points[1], sizeof(f32) * NumPoints * (NumPoints-1));

    // init new points
    for (u32 y = 0; y < NumPoints; y++)
        Points[NumPoints-1][y] = 1.5;

    Position += core::vector3df(1, 0, 0);
    regenerate();
}

void ObjectGrid::addMinusY()
{

}
