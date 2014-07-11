#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont),
    Generator(NumPointsX),
    ColorChangeLast(30),
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
    //BackNode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);


    regenerate();
}

ObjectGrid::~ObjectGrid()
{
    Node->remove();
    BackNode->remove();

    Object* pl = Context->ObjManager->getObjectFromName("ObjectPlayer");
    if (pl)
        pl->unregisterObserver(this);

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

        if (handleCollision(core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z), diffVect))
            return;

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

                handleColors();
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

            distModX.X = thicknessCorrection;
            distModY.Y = thicknessCorrection;
            distModZ.Z = thicknessCorrection;

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
}


void ObjectGrid::addX()
{
    Generator.reset();

    for (u32 x = 0; x < NumPointsX; x++)
    {
        Generator.addPoint(Points[x][NumPointsY-1]);
        memmove(Points[x], &Points[x][1], sizeof(f32) * (NumPointsY-1));
    }


    Generator.generate(Position);

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

void ObjectGrid::handleColors()
{
    ColorChangeLast++;
    if (ColorChangeLast >= ColorChangeEvery)
    {
        do
            ColorNext = video::SColorf(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX);
        while (ColorNext.r + ColorNext.g + ColorNext.b <= 0.5);
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

bool ObjectGrid::handleCollision(core::vector3df pPos, core::vector3df diffV)
{
    u32 halfPtsX = NumPointsX / 2;
    u32 posXOffset = diffV.X < 0 ? 1 : 0;
    u32 posZOffset = diffV.Z > 0.5 ? 1 : 0;
    f32 posX = Position.X - posXOffset;
    f32 posZ = Position.Z + posZOffset - 0.5;

    core::vector3df ld(posX, 0, posZ);
    ld.Y = Points[halfPtsX - posXOffset][posZOffset];

    core::vector3df rd(posX + 1, 0, posZ);
    rd.Y = Points[halfPtsX - posXOffset + 1][posZOffset];

    core::vector3df lu(posX, 0, posZ + 1);
    lu.Y = Points[halfPtsX - posXOffset][posZOffset + 1];

    core::vector3df ru(posX + 1, 0, posZ + 1);
    ru.Y = Points[halfPtsX - posXOffset + 1][posZOffset + 1];


    core::triangle3df t1, t2;

    if (ld.Y + ru.Y <= rd.Y + lu.Y)
    {
        t1.pointA = rd;
        t1.pointB = ld;
        t1.pointC = ru;
        t2.pointA = ld;
        t2.pointB = lu;
        t2.pointC = ru;
    }
    else
    {
        t1.pointA = rd;
        t1.pointB = ld;
        t1.pointC = lu;
        t2.pointA = rd;
        t2.pointB = lu;
        t2.pointC = ru;
    }

    core::line3df t1l(pPos, pPos -999 * t1.getNormal());
    core::line3df t2l(pPos, pPos -999 * t2.getNormal());

    core::vector3df t1Int, t2Int;
    if (t1.getIntersectionWithLimitedLine(t1l, t1Int))
    {
        t1l.end = t1Int;
        if (t1l.getLength() < PLayerSize)
        {
            Context->ObjManager->broadcastMessage(SMessage(this, EMT_PLAYER_CRASHED));
            return true;
        }

#ifdef DEBUG_PLAYER
    Context->Device->getVideoDriver()->setMaterial(video::SMaterial());
    Context->Device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    Context->Device->getVideoDriver()->draw3DLine(t1l.start, t1l.end);
#endif // DEBUG_PLAYER
    }
    if (t2.getIntersectionWithLimitedLine(t2l, t2Int))
    {
        t2l.end = t2Int;
        if (t2l.getLength() < PLayerSize)
        {
            Context->ObjManager->broadcastMessage(SMessage(this, EMT_PLAYER_CRASHED));
            return true;
        }

#ifdef DEBUG_PLAYER
    Context->Device->getVideoDriver()->setMaterial(video::SMaterial());
    Context->Device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
    Context->Device->getVideoDriver()->draw3DLine(t2l.start, t2l.end);
#endif // DEBUG_PLAYER
    }

    return false;
}
