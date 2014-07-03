#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont),
    Generator(NumPointsX),
    ColorChangeLast(0),
    ChangingColor(0)
{
    Name = "ObjectGrid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Position = core::vector3df(0);

    for (u32 x = 0; x < NumPointsX; x++)
        for (u32 y = 0; y < NumPointsY; y++)
            Points[x][y] = 0;

    Generator.setType(EGT_PLAINS);

    Points[10][15] = 2;
    Points[11][15] = 1;
    Points[11][14] = 0.4;
    Points[12][15] = 1;
    Points[20][20] = 1;
    Points[21][19] = 1;

    Buffer = new scene::SMeshBuffer();
    scene::SMesh* mesh = new scene::SMesh();
    mesh->setHardwareMappingHint(scene::EHM_STATIC);
    mesh->addMeshBuffer(Buffer);
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->Grid);
    Node->setAutomaticCulling(scene::EAC_OFF);

    BufferAppx = new scene::SMeshBuffer();
    scene::SMesh* backMesh = new scene::SMesh();
    backMesh->setHardwareMappingHint(scene::EHM_STATIC);
    backMesh->addMeshBuffer(BufferAppx);
    BackNode = Context->Device->getSceneManager()->addMeshSceneNode(backMesh);
    BackNode->setMaterialType(Context->Mtls->GridBack);
    BackNode->setAutomaticCulling(scene::EAC_OFF);
    BackNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);


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

            if (diffVect.X > 0.5)
                addPlusY();
            else if (diffVect.X < -0.5)
                addMinusY();

            if (diffVect.Z > 0.5)
            {
                addX();

                ColorChangeLast++;
                if (ColorChangeLast >= ColorChangeEvery)
                {
                    ColorNext = video::SColorf(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX);
                    ColorFar = Context->Mtls->GridCB->getFarColor();
                    ChangingColor = NumPointsY;
                    ColorChangeLast = 0;

#ifdef DEBUG_GRID
                    debugLog("Starting color change");
#endif // DEBUG_GRID
                }
                if (ChangingColor > 0)
                {
                    ShaderCBGrid* callback = Context->Mtls->GridCB;

                    video::SColorf near = callback->getNearColor();
                    video::SColorf far = callback->getFarColor();

                    near = ColorFar.getInterpolated(near, 1.0 / (NumPointsY-(NumPointsY-ChangingColor)));
                    far = ColorNext.getInterpolated(far, 1.0 / (NumPointsY-(NumPointsY-ChangingColor)));

                    callback->setNearColor(near);
                    callback->setFarColor(far);
                    ChangingColor--;
                }
            }

            regenerate();



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

    core::vector3df center(NumPointsX/2.0, 0, 0.5);
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
    for (u32 y = 0; y < NumPointsY; y++)
    {
        pointVec.Z = y - center.Z;

        for (u32 x = 0; x < NumPointsX; x++)
        {
            pointVec.X = x - center.X;
            pointVec.Y = Points[x][y];

            /*f32 thicknessCorrection = (x-NumPoints/2.0)/NumPoints;// + ((y-NumPoints/2.0)/NumPoints);
            thicknessCorrection *= 0.1;
            if (thicknessCorrection < 0) thicknessCorrection *= -1;
            thicknessCorrection += LineThickness;*/

            distModX.X = thicknessCorrection;
            distModY.Y = thicknessCorrection;
            distModZ.Z = thicknessCorrection;

            /*Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModX, core::vector3df(1, 0, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModX, core::vector3df(-1, 0, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModY, core::vector3df(0, 1, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModY, core::vector3df(0, -1, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec + distModZ, core::vector3df(0, 0, 1), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec - distModZ, core::vector3df(0, 0, -1), white, null2d));*/
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(1, 0, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(-1, 0, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(0, 1, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(0, -1, 0), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(0, 0, 1), white, null2d));
            Buffer->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(0, 0, -1), white, null2d));

            BufferAppx->Vertices.push_back(video::S3DVertex(pointVec, core::vector3df(0, -1, 0), black, null2d));

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
                const u32 prevYVertC = ((y - 1) * NumPointsX + x) * 6;
                //X quad
                Buffer->Indices.push_back(vertC-5); Buffer->Indices.push_back(vertC-6); Buffer->Indices.push_back(prevYVertC+1);
                Buffer->Indices.push_back(vertC-6); Buffer->Indices.push_back(prevYVertC); Buffer->Indices.push_back(prevYVertC+1);
                //Y quad
                Buffer->Indices.push_back(vertC-4); Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(prevYVertC+2);
                Buffer->Indices.push_back(vertC-3); Buffer->Indices.push_back(prevYVertC+3); Buffer->Indices.push_back(prevYVertC+2);
            }
            if (x > 0 && y > 0)
            {
                const u32 vertC = y * NumPointsX + x;
                const u32 prevYVertC = vertC - NumPointsX;

                if (Points[x][y] + Points[x-1][y-1] > Points[x-1][y] + Points[x][y-1])
                {
                    BufferAppx->Indices.push_back(vertC); BufferAppx->Indices.push_back(prevYVertC); BufferAppx->Indices.push_back(vertC-1);
                    BufferAppx->Indices.push_back(prevYVertC-1); BufferAppx->Indices.push_back(vertC-1); BufferAppx->Indices.push_back(prevYVertC);
                }
                else
                {
                    BufferAppx->Indices.push_back(vertC); BufferAppx->Indices.push_back(prevYVertC); BufferAppx->Indices.push_back(prevYVertC-1);
                    BufferAppx->Indices.push_back(prevYVertC-1); BufferAppx->Indices.push_back(vertC-1); BufferAppx->Indices.push_back(vertC);
                }
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


void ObjectGrid::addX()
{
    Generator.reset();

    for (u32 x = 0; x < NumPointsX; x++)
    {
        Generator.addPoint(Points[x][NumPointsY-1]);
        memmove(Points[x], &Points[x][1], sizeof(f32) * (NumPointsY-1));
    }


    Generator.generate();

    for (u32 x = 0; x < NumPointsX; x++)
        Points[x][NumPointsY-1] = Generator.getGenerated(x);

    Position += core::vector3df(0, 0, 1);
}

void ObjectGrid::addPlusY()
{
    memmove(Points, Points[1], sizeof(f32) * NumPointsY * (NumPointsX-1));

    for (u32 y = 0; y < NumPointsY; y++)
    {
        Points[NumPointsX - 1][y] = Points[NumPointsX - 2][y];
    }

    Position += core::vector3df(1, 0, 0);
}

void ObjectGrid::addMinusY()
{
    memmove(Points[1], Points, sizeof(f32) * NumPointsY * (NumPointsX-1));

    for (u32 y = 0; y < NumPointsY; y++)
    {
        Points[0][y] = Points[1][y];
    }

    Position += core::vector3df(-1, 0, 0);
}
