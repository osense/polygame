#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : ObjectVisual(cont)
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
    Points[2][20] = -0.5;

    Buffer = new scene::SMeshBuffer();
    scene::SMesh* mesh = new scene::SMesh();
    mesh->setHardwareMappingHint(scene::EHM_NEVER);
    mesh->addMeshBuffer(Buffer);
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->Grid);
    Node->setAutomaticCulling(scene::EAC_OFF);

    BufferAppx = new scene::SMeshBuffer();
    scene::SMesh* backMesh = new scene::SMesh();
    backMesh->setHardwareMappingHint(scene::EHM_NEVER);
    backMesh->addMeshBuffer(BufferAppx);
    BackNode = Context->Device->getSceneManager()->addMeshSceneNode(backMesh);
    BackNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    BackNode->setMaterialType(Context->Mtls->Solid);
    BackNode->setAutomaticCulling(scene::EAC_OFF);


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
    BufferAppx->Vertices.clear();
    BufferAppx->Indices.clear();


    video::SColor white(255, 255, 255, 255), black(255, 0, 0, 0);
    core::vector2df null2d(0);
    core::vector3df null3d(0);

    core::vector3df center(NumPoints/2.0, 0, NumPoints/2.0);
    center -= Position;
    center.Y = 0;

    core::vector3df pointVec(0, Points[0][0], 0);
    pointVec -= center;

    f32 thicknessCorrection = 0;
    thicknessCorrection += LineThickness;
    core::vector3df distModX(thicknessCorrection, 0, 0);
    core::vector3df distModY(0, thicknessCorrection, 0);
    core::vector3df distModZ(0, 0, thicknessCorrection);


    // iterate the grid, spawning vertices and faces
    for (u32 y = 0; y < NumPoints; y++)
    {
        pointVec.Z = y - center.Z;

        for (u32 x = 0; x < NumPoints; x++)
        {
            pointVec.X = x - center.X;
            pointVec.Y = Points[x][y];

            f32 thicknessCorrection = 0;// = ((x-NumPoints/2.0)/NumPoints) * ((y-NumPoints/2.0)/NumPoints);//  //((i*j - ((NumPoints*NumPoints)/2.0)) / (NumPoints*NumPoints));
            //thicknessCorrection *= 0.1;
            //if (thicknessCorrection < 0) thicknessCorrection *= -1;
            thicknessCorrection += LineThickness;

            distModX.X = thicknessCorrection;
            distModY.Y = thicknessCorrection;
            distModZ.Z = thicknessCorrection;

            Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModX, null3d, white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModX, null3d, white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModY, null3d, white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModY, null3d, white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModZ, null3d, white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModZ, null3d, white, null2d));

            BufferAppx->Vertices.push_back(video::S3DVertex(pointVec - distModY, null3d, black, null2d));

            if (x > 0)
            {
                const u32 vertC = Buffer->Vertices.size();
                //Y quad
                Buffer->Indices.push_back(vertC-4); Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(vertC-10);
                Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(vertC-9); Buffer->Indices.push_back(vertC-10);
                //Z quad
                Buffer->Indices.push_back(vertC-2); Buffer->Indices.push_back(vertC-1); Buffer->Indices.push_back(vertC-8);
                Buffer->Indices.push_back(vertC-1); Buffer->Indices.push_back(vertC-7); Buffer->Indices.push_back(vertC-8);
            }
            if (y > 0)
            {
                const u32 vertC = Buffer->Vertices.size();
                const u32 prevYVertC = ((y - 1) * NumPoints + x) * 6;
                //X quad
                Buffer->Indices.push_back(vertC-5); Buffer->Indices.push_back(vertC-6); Buffer->Indices.push_back(prevYVertC+1);
                Buffer->Indices.push_back(vertC-6); Buffer->Indices.push_back(prevYVertC); Buffer->Indices.push_back(prevYVertC+1);
                //Y quad
                Buffer->Indices.push_back(vertC-4); Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(prevYVertC+2);
                Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(prevYVertC+3); Buffer->Indices.push_back(prevYVertC+2);
            }
            if (x > 0 && y > 0)
            {
                const u32 vertC = y * NumPoints + x;
                const u32 prevYVertC = vertC - NumPoints;
                BufferAppx->Indices.push_back(vertC); BufferAppx->Indices.push_back(prevYVertC); BufferAppx->Indices.push_back(prevYVertC-1);
                BufferAppx->Indices.push_back(vertC); BufferAppx->Indices.push_back(prevYVertC-1); BufferAppx->Indices.push_back(vertC-1);
            }

        }
    }

    Buffer->setDirty();
    BufferAppx->setDirty();
    //Buffer->recalculateBoundingBox();

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
        Points[x][NumPoints-1] = Points[x][NumPoints-2] + (Context->Device->getRandomizer()->frand()-0.5);

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
