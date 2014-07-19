#include "ObjectItemCube.h"

ObjectItemCube::ObjectItemCube(SContext* cont, core::vector3df pos) : Object(cont)
{
    Name = "ObjectItemCube";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    scene::ISceneManager* smgr = Context->Device->getSceneManager();
    Node = smgr->addMeshSceneNode(getCubeMesh());
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->ItemCube);

    Node->setPosition(pos);
}

ObjectItemCube::~ObjectItemCube()
{
    Node->remove();

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectItemCube::onMessage(SMessage msg)
{

}


scene::IMesh* ObjectItemCube::getCubeMesh() const
{
    scene::IAnimatedMesh* mesh = Context->Device->getSceneManager()->getMesh("cube-mesh");
    if (mesh)
        return mesh;

    mesh = new scene::SAnimatedMesh;
    scene::SMesh* tmpMesh = new scene::SMesh();
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
    tmpMesh->addMeshBuffer(buffer);
    static_cast<scene::SAnimatedMesh*>(mesh)->addMesh(tmpMesh);
    Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh", mesh);


    video::SColor white(255, 255, 255, 255);
    core::vector2df null2d(0, 0);

    // up
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, Size), core::vector3df(0, -1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, Size), core::vector3df(-1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, Size), core::vector3df(-1, -1, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, Size), core::vector3df(0, -1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, Size), core::vector3df(1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, Size), core::vector3df(1, -1, 0), white, null2d));
    buffer->Indices.push_back(0); buffer->Indices.push_back(4); buffer->Indices.push_back(7);
    buffer->Indices.push_back(0); buffer->Indices.push_back(7); buffer->Indices.push_back(3);
    buffer->Indices.push_back(0); buffer->Indices.push_back(2); buffer->Indices.push_back(4);
    buffer->Indices.push_back(2); buffer->Indices.push_back(6); buffer->Indices.push_back(4);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, -Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, -Size), core::vector3df(0, -1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, -Size), core::vector3df(1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, Size, -Size), core::vector3df(1, -1, 0), white, null2d));
    buffer->Indices.push_back(4); buffer->Indices.push_back(9); buffer->Indices.push_back(5);
    buffer->Indices.push_back(4); buffer->Indices.push_back(8); buffer->Indices.push_back(9);
    buffer->Indices.push_back(4); buffer->Indices.push_back(6); buffer->Indices.push_back(8);
    buffer->Indices.push_back(6); buffer->Indices.push_back(10); buffer->Indices.push_back(8);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, -Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, -Size), core::vector3df(0, -1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, -Size), core::vector3df(-1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, Size, -Size), core::vector3df(-1, -1, 0), white, null2d));
    buffer->Indices.push_back(8); buffer->Indices.push_back(15); buffer->Indices.push_back(11);
    buffer->Indices.push_back(8); buffer->Indices.push_back(12); buffer->Indices.push_back(15);
    buffer->Indices.push_back(8); buffer->Indices.push_back(10); buffer->Indices.push_back(12);
    buffer->Indices.push_back(10); buffer->Indices.push_back(14); buffer->Indices.push_back(12);
    buffer->Indices.push_back(12); buffer->Indices.push_back(1); buffer->Indices.push_back(13);
    buffer->Indices.push_back(12); buffer->Indices.push_back(0); buffer->Indices.push_back(1);
    buffer->Indices.push_back(12); buffer->Indices.push_back(14); buffer->Indices.push_back(0);
    buffer->Indices.push_back(14); buffer->Indices.push_back(2); buffer->Indices.push_back(0);

    // down
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, Size), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, Size), core::vector3df(-1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, Size), core::vector3df(-1, 1, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, Size), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, Size), core::vector3df(1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, Size), core::vector3df(1, 1, 0), white, null2d));
    buffer->Indices.push_back(16); buffer->Indices.push_back(19); buffer->Indices.push_back(20);
    buffer->Indices.push_back(19); buffer->Indices.push_back(23); buffer->Indices.push_back(20);
    buffer->Indices.push_back(16); buffer->Indices.push_back(20); buffer->Indices.push_back(22);
    buffer->Indices.push_back(16); buffer->Indices.push_back(22); buffer->Indices.push_back(18);

    buffer->Indices.push_back(16); buffer->Indices.push_back(0); buffer->Indices.push_back(3);
    buffer->Indices.push_back(16); buffer->Indices.push_back(3); buffer->Indices.push_back(19);
    buffer->Indices.push_back(16); buffer->Indices.push_back(17); buffer->Indices.push_back(1);
    buffer->Indices.push_back(16); buffer->Indices.push_back(1); buffer->Indices.push_back(0);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, -Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, -Size), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, -Size), core::vector3df(1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-Size, -Size, -Size), core::vector3df(1, 1, 0), white, null2d));
    buffer->Indices.push_back(20); buffer->Indices.push_back(21); buffer->Indices.push_back(24);
    buffer->Indices.push_back(21); buffer->Indices.push_back(25); buffer->Indices.push_back(24);
    buffer->Indices.push_back(20); buffer->Indices.push_back(24); buffer->Indices.push_back(26);
    buffer->Indices.push_back(20); buffer->Indices.push_back(26); buffer->Indices.push_back(22);

    buffer->Indices.push_back(20); buffer->Indices.push_back(23); buffer->Indices.push_back(4);
    buffer->Indices.push_back(23); buffer->Indices.push_back(7); buffer->Indices.push_back(4);
    buffer->Indices.push_back(20); buffer->Indices.push_back(4); buffer->Indices.push_back(21);
    buffer->Indices.push_back(21); buffer->Indices.push_back(4); buffer->Indices.push_back(5);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, -Size), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, -Size), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, -Size), core::vector3df(-1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(Size, -Size, -Size), core::vector3df(-1, 1, 0), white, null2d));
    buffer->Indices.push_back(24); buffer->Indices.push_back(27); buffer->Indices.push_back(28);
    buffer->Indices.push_back(27); buffer->Indices.push_back(31); buffer->Indices.push_back(28);
    buffer->Indices.push_back(24); buffer->Indices.push_back(28); buffer->Indices.push_back(30);
    buffer->Indices.push_back(24); buffer->Indices.push_back(30); buffer->Indices.push_back(26);
    buffer->Indices.push_back(28); buffer->Indices.push_back(29); buffer->Indices.push_back(16);
    buffer->Indices.push_back(29); buffer->Indices.push_back(17); buffer->Indices.push_back(16);
    buffer->Indices.push_back(28); buffer->Indices.push_back(16); buffer->Indices.push_back(18);
    buffer->Indices.push_back(28); buffer->Indices.push_back(18); buffer->Indices.push_back(30);

    buffer->Indices.push_back(24); buffer->Indices.push_back(8); buffer->Indices.push_back(11);
    buffer->Indices.push_back(24); buffer->Indices.push_back(11); buffer->Indices.push_back(27);
    buffer->Indices.push_back(24); buffer->Indices.push_back(25); buffer->Indices.push_back(8);
    buffer->Indices.push_back(25); buffer->Indices.push_back(9); buffer->Indices.push_back(8);
    buffer->Indices.push_back(28); buffer->Indices.push_back(31); buffer->Indices.push_back(12);
    buffer->Indices.push_back(31); buffer->Indices.push_back(15); buffer->Indices.push_back(12);
    buffer->Indices.push_back(28); buffer->Indices.push_back(12); buffer->Indices.push_back(13);
    buffer->Indices.push_back(28); buffer->Indices.push_back(13); buffer->Indices.push_back(29);


    return mesh;
}
