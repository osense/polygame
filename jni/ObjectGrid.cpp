#include "ObjectGrid.h"

ObjectGrid::ObjectGrid(SContext* cont) : Object(cont),
    BaseHeight(NumPointsY),
    CollisionActive(true),
    Generator(NumPointsX),
    GenChangeIn(GenChangeEvery),
    ColorChangeIn(5),
    ChangingColor(0)
{
    Name = "ObjectGrid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Position = core::vector3df(0);

    for (u32 y = 0; y < NumPointsY; y++)
    {
        for (u32 x = 0; x < NumPointsX; x++)
            Points[x][y] = 0;

        BaseHeight.push_back(0);
    }

    srand(0);
    Generator.setType(EGT_PLAINS);
    Generator.setSlope(EST_DOWN);

    Context->Mtls->GridCB->setNearColor(video::SColorf(1, 1, 0));
    Context->Mtls->GridCB->setFarColor(video::SColorf(1, 1, 0));
    Context->Mtls->GridBackCB->setAlpha(1);

    Points[10][15] = 2;
    Points[11][15] = 1;
    Points[11][14] = 0.4;
    Points[12][15] = 1;

    Buffer = new scene::SMeshBuffer();
    scene::SMesh* mesh = new scene::SMesh();
    mesh->setHardwareMappingHint(scene::EHM_STATIC);
    mesh->addMeshBuffer(Buffer);
    Buffer->drop();
    Node = Context->Device->getSceneManager()->addMeshSceneNode(mesh);
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->Grid);
    Node->setAutomaticCulling(scene::EAC_OFF);
    mesh->drop();

    BufferAppx = new scene::SMeshBuffer();
    scene::SMesh* backMesh = new scene::SMesh();
    backMesh->setHardwareMappingHint(scene::EHM_STATIC);
    backMesh->addMeshBuffer(BufferAppx);
    BufferAppx->drop();
    BackNode = Context->Device->getSceneManager()->addMeshSceneNode(backMesh);
    BackNode->setMaterialType(Context->Mtls->GridBack);
    BackNode->setAutomaticCulling(scene::EAC_OFF);
    backMesh->drop();


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
        {
            Context->ObjManager->broadcastMessage(SMessage(this, EMT_PLAYER_CRASHED));
            return;
        }

        // handle all kinds of update stuff
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

                handleGenUpdate();

                handleColors();
            }

            regenerate();
            broadcastMessage(SMessage(this, EMT_GRID_REGENED));

#ifdef DEBUG_GRID
            u32 updEnd = Context->Device->getTimer()->getTime();
            debugLog(core::stringc("Updated grid: ") + core::stringc(updStart - updEnd) + "ms");
#endif // DEBUG_GRID
        }
    }
}

void ObjectGrid::setCollision(bool active)
{
    CollisionActive = active;
}

core::vector3df ObjectGrid::getPosition() const
{
    return Position;
}

u32 ObjectGrid::getNumPointsX() const
{
    return NumPointsX;
}

u32 ObjectGrid::getNumPointsY() const
{
    return NumPointsY;
}

f32 ObjectGrid::getBaseHeight(u32 y) const
{
    return BaseHeight[BaseHeight.getIndex() + y + 1];
}

f32 ObjectGrid::getHillHeight(u32 x, u32 y) const
{
    return Points[x][y] - getBaseHeight(y);
}

const GridGenerator& ObjectGrid::getGenerator() const
{
    return Generator;
}


void ObjectGrid::regenerate()
{
    Buffer->Vertices.clear();
    Buffer->Indices.clear();
    BufferAppx->Vertices.clear();
    BufferAppx->Indices.clear();


    video::SColor white(255, 255, 255, 255), black(255, 0, 0, 0);
    core::vector2df null2d(0);
    core::vector3df null3d(0);

    core::vector3df center(NumPointsX/2.0, 0, 1);
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
        Points[NumPointsX - 1][y] = getBaseHeight(y);
    }

    Position += core::vector3df(1, 0, 0);
}

void ObjectGrid::addMinusY()
{
    memmove(Points[1], Points, sizeof(f32) * NumPointsY * (NumPointsX-1));

    for (u32 y = 0; y < NumPointsY; y++)
    {
        Points[0][y] = getBaseHeight(y);
    }

    Position += core::vector3df(-1, 0, 0);
}

void ObjectGrid::handleGenUpdate()
{
    BaseHeight.push_back(Generator.getHeight());
    //Context->Device->getSceneManager()->addCubeSceneNode(0.1, 0, -1, core::vector3df(Position.X, getBaseHeight(NumPointsY - 1), Position.Z+NumPointsY-1));

    Generator.setDifficulty(Generator.getDifficulty() + 0.5 / GenChangeEvery);

    GenChangeIn--;

    if (GenChangeIn <= 0)
    {
        Generator.setType(E_GEN_TYPE(rand()%EGT_COUNT));
        Generator.setDifficulty(Generator.getDifficulty() - 0.45);
        GenChangeIn = GenChangeEvery;
    }

    /*if (GenChangeIn == int(0.8 * GenChangeEvery))
    {
        Generator.setSlope(EST_DOWN);
    }*/
    if (GenChangeIn == int(0.6 * GenChangeEvery))
    {
        Generator.setSlope(EST_NONE);
    }
}

void ObjectGrid::handleColors()
{
    ColorChangeIn--;
    if (ColorChangeIn == 0)
    {
        //do
            ColorNext = hueShift(Context->Mtls->GridCB->getFarColor(), 240);// video::SColorf(rand()/(float)RAND_MAX, rand()/(float)RAND_MAX, rand()/(float)RAND_MAX);
        //while (ColorNext.r + ColorNext.g + ColorNext.b <= 0.5);
        ColorFar = Context->Mtls->GridCB->getFarColor();
        ChangingColor = NumPointsY;
        ColorChangeIn = ColorChangeEvery;

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
    SMessage msg(this, EMT_PLAYER_FEEDBACK);

    msg.PlayerFeedback.GridAngle = -radToDeg(atan(getBaseHeight(2) - getBaseHeight(1)));
    msg.PlayerFeedback.Height = getBaseHeight(1) + (getBaseHeight(2) - getBaseHeight(1)) * (pPos.Z - Position.Z);

    // "hack"
    Context->ObjManager->getObjectFromName("ObjectPlayer")->onMessage(msg);


    if (!CollisionActive)
        return false;


    u32 halfPtsX = NumPointsX / 2;
    u32 posXOffset = diffV.X < 0 ? 1 : 0;
    u32 posZOffset = 1;//diffV.Z > 0.5 ? 1 : 0;
    f32 posX = Position.X - posXOffset;
    f32 posZ = Position.Z;

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

    core::vector3df t1n = t1.getNormal().normalize();
    core::vector3df t2n = t2.getNormal().normalize();
    core::line3df t1l(pPos + t1n, pPos - 999 * t1n);
    core::line3df t2l(pPos + t2n, pPos - 999 * t2n);

#ifdef DEBUG_PLAYER
        Context->Device->getVideoDriver()->setMaterial(video::SMaterial());
        Context->Device->getVideoDriver()->setTransform(video::ETS_WORLD, core::IdentityMatrix);
        Context->Device->getVideoDriver()->draw3DTriangle(t1);
        Context->Device->getVideoDriver()->draw3DTriangle(t2);
#endif // DEBUG_PLAYER

    core::vector3df t1Int, t2Int;
    if (t1.getIntersectionWithLimitedLine(t1l, t1Int))
    {
        if ((pPos - t1Int).getLength() < PlayerSize)
        {
            return true;
        }

#ifdef DEBUG_PLAYER
        Context->Device->getVideoDriver()->draw3DLine(pPos, t1Int);
#endif // DEBUG_PLAYER
    }
    if (t2.getIntersectionWithLimitedLine(t2l, t2Int))
    {
        if ((pPos - t2Int).getLength() < PlayerSize)
        {
            return true;
        }

#ifdef DEBUG_PLAYER
        Context->Device->getVideoDriver()->draw3DLine(pPos, t2Int);
#endif // DEBUG_PLAYER
    }

    return false;
}
