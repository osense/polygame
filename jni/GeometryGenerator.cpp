#include "GeometryGenerator.h"

scene::IMesh* GeometryGenerator::createLineMesh(f32 size) const
{
    scene::IAnimatedMesh* mesh = new scene::SAnimatedMesh;
    scene::SMesh* tmpMesh = new scene::SMesh();
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
    tmpMesh->addMeshBuffer(buffer);
    buffer->drop();
    static_cast<scene::SAnimatedMesh*>(mesh)->addMesh(tmpMesh);
    tmpMesh->drop();

    video::SColor white(255, 255, 255, 255);
    core::vector2df null2d(0, 0);
    f32 cs = size;

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, 0), core::vector3df(0, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, 0), core::vector3df(0, -1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, 0), core::vector3df(-1, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, 0), core::vector3df(1, 0, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, -cs), core::vector3df(0, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, -cs), core::vector3df(0, -1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, -cs), core::vector3df(-1, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, 0, -cs), core::vector3df(1, 0, 0), white, null2d));

    buffer->Indices.push_back(0); buffer->Indices.push_back(4); buffer->Indices.push_back(5);
    buffer->Indices.push_back(0); buffer->Indices.push_back(5); buffer->Indices.push_back(1);
    buffer->Indices.push_back(2); buffer->Indices.push_back(3); buffer->Indices.push_back(6);
    buffer->Indices.push_back(3); buffer->Indices.push_back(7); buffer->Indices.push_back(6);


    static_cast<scene::SAnimatedMesh*>(mesh)->recalculateBoundingBox();
    return mesh;
}

scene::IMesh* GeometryGenerator::createCubeMesh(f32 size, bool filled) const
{
    scene::IAnimatedMesh* mesh = new scene::SAnimatedMesh;
    scene::SMesh* tmpMesh = new scene::SMesh();
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
    tmpMesh->addMeshBuffer(buffer);
    buffer->drop();
    static_cast<scene::SAnimatedMesh*>(mesh)->addMesh(tmpMesh);
    tmpMesh->drop();

    video::SColor white(255, 255, 255, 255);
    core::vector2df null2d(0, 0);
    f32 cs = size;

    if (filled)
    {
        scene::SMeshBuffer* fillBuffer = new scene::SMeshBuffer();
        tmpMesh->addMeshBuffer(fillBuffer);
        fillBuffer->drop();
        video::SColor black(255, 0, 0, 0);

        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, cs), core::vector3df(-1, 0, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, cs), core::vector3df(1, 0, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, -cs), core::vector3df(1, 0, 1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, -cs), core::vector3df(-1, 0, 1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, cs), core::vector3df(-1, 0, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, cs), core::vector3df(1, 0, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, -cs), core::vector3df(1, 0, 1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, -cs), core::vector3df(-1, 0, 1), black, null2d));

        fillBuffer->Indices.push_back(0); fillBuffer->Indices.push_back(4); fillBuffer->Indices.push_back(1);
        fillBuffer->Indices.push_back(4); fillBuffer->Indices.push_back(5); fillBuffer->Indices.push_back(1);
        fillBuffer->Indices.push_back(1); fillBuffer->Indices.push_back(5); fillBuffer->Indices.push_back(2);
        fillBuffer->Indices.push_back(5); fillBuffer->Indices.push_back(6); fillBuffer->Indices.push_back(2);
        fillBuffer->Indices.push_back(2); fillBuffer->Indices.push_back(3); fillBuffer->Indices.push_back(7);
        fillBuffer->Indices.push_back(7); fillBuffer->Indices.push_back(6); fillBuffer->Indices.push_back(2);
        fillBuffer->Indices.push_back(3); fillBuffer->Indices.push_back(0); fillBuffer->Indices.push_back(4);
        fillBuffer->Indices.push_back(4); fillBuffer->Indices.push_back(7); fillBuffer->Indices.push_back(3);
    }

    // up
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, cs), core::vector3df(0, -1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, cs), core::vector3df(-1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, cs), core::vector3df(-1, -1, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, cs), core::vector3df(0, -1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, cs), core::vector3df(1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, cs), core::vector3df(1, -1, 0), white, null2d));
    buffer->Indices.push_back(0); buffer->Indices.push_back(4); buffer->Indices.push_back(7);
    buffer->Indices.push_back(0); buffer->Indices.push_back(7); buffer->Indices.push_back(3);
    buffer->Indices.push_back(0); buffer->Indices.push_back(2); buffer->Indices.push_back(4);
    buffer->Indices.push_back(2); buffer->Indices.push_back(6); buffer->Indices.push_back(4);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, -cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, -cs), core::vector3df(0, -1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, -cs), core::vector3df(1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, cs, -cs), core::vector3df(1, -1, 0), white, null2d));
    buffer->Indices.push_back(4); buffer->Indices.push_back(9); buffer->Indices.push_back(5);
    buffer->Indices.push_back(4); buffer->Indices.push_back(8); buffer->Indices.push_back(9);
    buffer->Indices.push_back(4); buffer->Indices.push_back(6); buffer->Indices.push_back(8);
    buffer->Indices.push_back(6); buffer->Indices.push_back(10); buffer->Indices.push_back(8);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, -cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, -cs), core::vector3df(0, -1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, -cs), core::vector3df(-1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, cs, -cs), core::vector3df(-1, -1, 0), white, null2d));
    buffer->Indices.push_back(8); buffer->Indices.push_back(15); buffer->Indices.push_back(11);
    buffer->Indices.push_back(8); buffer->Indices.push_back(12); buffer->Indices.push_back(15);
    buffer->Indices.push_back(8); buffer->Indices.push_back(10); buffer->Indices.push_back(12);
    buffer->Indices.push_back(10); buffer->Indices.push_back(14); buffer->Indices.push_back(12);
    buffer->Indices.push_back(12); buffer->Indices.push_back(1); buffer->Indices.push_back(13);
    buffer->Indices.push_back(12); buffer->Indices.push_back(0); buffer->Indices.push_back(1);
    buffer->Indices.push_back(12); buffer->Indices.push_back(14); buffer->Indices.push_back(0);
    buffer->Indices.push_back(14); buffer->Indices.push_back(2); buffer->Indices.push_back(0);

    // down
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, cs), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, cs), core::vector3df(-1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, cs), core::vector3df(-1, 1, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, cs), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, cs), core::vector3df(1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, cs), core::vector3df(1, 1, 0), white, null2d));
    buffer->Indices.push_back(16); buffer->Indices.push_back(19); buffer->Indices.push_back(20);
    buffer->Indices.push_back(19); buffer->Indices.push_back(23); buffer->Indices.push_back(20);
    buffer->Indices.push_back(16); buffer->Indices.push_back(20); buffer->Indices.push_back(22);
    buffer->Indices.push_back(16); buffer->Indices.push_back(22); buffer->Indices.push_back(18);

    buffer->Indices.push_back(16); buffer->Indices.push_back(0); buffer->Indices.push_back(3);
    buffer->Indices.push_back(16); buffer->Indices.push_back(3); buffer->Indices.push_back(19);
    buffer->Indices.push_back(16); buffer->Indices.push_back(17); buffer->Indices.push_back(1);
    buffer->Indices.push_back(16); buffer->Indices.push_back(1); buffer->Indices.push_back(0);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, -cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, -cs), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, -cs), core::vector3df(1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -cs, -cs), core::vector3df(1, 1, 0), white, null2d));
    buffer->Indices.push_back(20); buffer->Indices.push_back(21); buffer->Indices.push_back(24);
    buffer->Indices.push_back(21); buffer->Indices.push_back(25); buffer->Indices.push_back(24);
    buffer->Indices.push_back(20); buffer->Indices.push_back(24); buffer->Indices.push_back(26);
    buffer->Indices.push_back(20); buffer->Indices.push_back(26); buffer->Indices.push_back(22);

    buffer->Indices.push_back(20); buffer->Indices.push_back(23); buffer->Indices.push_back(4);
    buffer->Indices.push_back(23); buffer->Indices.push_back(7); buffer->Indices.push_back(4);
    buffer->Indices.push_back(20); buffer->Indices.push_back(4); buffer->Indices.push_back(21);
    buffer->Indices.push_back(21); buffer->Indices.push_back(4); buffer->Indices.push_back(5);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, -cs), core::vector3df(0, 0, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, -cs), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, -cs), core::vector3df(-1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -cs, -cs), core::vector3df(-1, 1, 0), white, null2d));
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


    static_cast<scene::SAnimatedMesh*>(mesh)->recalculateBoundingBox();
    return mesh;
}

scene::IMesh* GeometryGenerator::createPyramidMesh(f32 size, bool filled) const
{
    scene::IAnimatedMesh* mesh = new scene::SAnimatedMesh;
    scene::SMesh* tmpMesh = new scene::SMesh();
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
    tmpMesh->addMeshBuffer(buffer);
    buffer->drop();
    static_cast<scene::SAnimatedMesh*>(mesh)->addMesh(tmpMesh);
    tmpMesh->drop();

    video::SColor white(255, 255, 255, 255);
    core::vector2df null2d(0, 0);
    f32 cs = size;
    f32 height = sqrt(2 * cs * cs);
    f32 heightHalf = height / 2;

    if (filled)
    {
        scene::SMeshBuffer* fillBuffer = new scene::SMeshBuffer();
        tmpMesh->addMeshBuffer(fillBuffer);
        fillBuffer->drop();
        video::SColor black(255, 0, 0, 0);

        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(-1, 1, 0), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(1, 1, 0), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(0, -1, 1), black, null2d));
        fillBuffer->Indices.push_back(0); fillBuffer->Indices.push_back(2); fillBuffer->Indices.push_back(1);

        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(0, 1, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(0, 1, 1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(-1, -1, 0), black, null2d));
        fillBuffer->Indices.push_back(3); fillBuffer->Indices.push_back(5); fillBuffer->Indices.push_back(4);

        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(1, 1, 0), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(-1, 1, 0), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(0, -1, -1), black, null2d));
        fillBuffer->Indices.push_back(6); fillBuffer->Indices.push_back(8); fillBuffer->Indices.push_back(7);

        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(0, 1, 1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(0, 1, -1), black, null2d));
        fillBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(1, -1, 0), black, null2d));
        fillBuffer->Indices.push_back(9); fillBuffer->Indices.push_back(11); fillBuffer->Indices.push_back(10);


    }

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(0, 0, 0), white, null2d)); // 0
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(-1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(-1, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, cs), core::vector3df(0, 1, 0), white, null2d));

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(0, 0, 0), white, null2d)); // 5
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(0, 1, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(1, 0, -1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(1, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, cs), core::vector3df(0, 1, 0), white, null2d));
    buffer->Indices.push_back(0); buffer->Indices.push_back(4); buffer->Indices.push_back(5);
    buffer->Indices.push_back(4); buffer->Indices.push_back(9); buffer->Indices.push_back(5);
    buffer->Indices.push_back(0); buffer->Indices.push_back(5); buffer->Indices.push_back(7);
    buffer->Indices.push_back(0); buffer->Indices.push_back(7); buffer->Indices.push_back(2);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(0, 0, 0), white, null2d)); // 10
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(1, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(-cs, -heightHalf, -cs), core::vector3df(0, 1, 0), white, null2d));
    buffer->Indices.push_back(5); buffer->Indices.push_back(9); buffer->Indices.push_back(10);
    buffer->Indices.push_back(9); buffer->Indices.push_back(14); buffer->Indices.push_back(10);
    buffer->Indices.push_back(5); buffer->Indices.push_back(7); buffer->Indices.push_back(10);
    buffer->Indices.push_back(7); buffer->Indices.push_back(12); buffer->Indices.push_back(10);

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(0, 0, 0), white, null2d)); // 15
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(0, 1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(-1, 0, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(-1, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(cs, -heightHalf, -cs), core::vector3df(0, 1, 0), white, null2d));
    buffer->Indices.push_back(10); buffer->Indices.push_back(14); buffer->Indices.push_back(15);
    buffer->Indices.push_back(14); buffer->Indices.push_back(19); buffer->Indices.push_back(15);
    buffer->Indices.push_back(10); buffer->Indices.push_back(12); buffer->Indices.push_back(15);
    buffer->Indices.push_back(12); buffer->Indices.push_back(17); buffer->Indices.push_back(15);

    buffer->Indices.push_back(15); buffer->Indices.push_back(19); buffer->Indices.push_back(0);
    buffer->Indices.push_back(19); buffer->Indices.push_back(4); buffer->Indices.push_back(0);
    buffer->Indices.push_back(15); buffer->Indices.push_back(17); buffer->Indices.push_back(0);
    buffer->Indices.push_back(17); buffer->Indices.push_back(2); buffer->Indices.push_back(0);

    // top
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(0, 0, 0), white, null2d)); // 20
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(1, -1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(-1, -1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(0, -1, 1), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, heightHalf, 0), core::vector3df(0, -1, -1), white, null2d));
    buffer->Indices.push_back(1); buffer->Indices.push_back(21); buffer->Indices.push_back(20);
    buffer->Indices.push_back(1); buffer->Indices.push_back(20); buffer->Indices.push_back(4);
    buffer->Indices.push_back(4); buffer->Indices.push_back(20); buffer->Indices.push_back(3);
    buffer->Indices.push_back(3); buffer->Indices.push_back(20); buffer->Indices.push_back(23);

    buffer->Indices.push_back(8); buffer->Indices.push_back(23); buffer->Indices.push_back(20);
    buffer->Indices.push_back(8); buffer->Indices.push_back(20); buffer->Indices.push_back(9);
    buffer->Indices.push_back(9); buffer->Indices.push_back(20); buffer->Indices.push_back(22);
    buffer->Indices.push_back(9); buffer->Indices.push_back(22); buffer->Indices.push_back(6);

    buffer->Indices.push_back(11); buffer->Indices.push_back(22); buffer->Indices.push_back(20);
    buffer->Indices.push_back(11); buffer->Indices.push_back(20); buffer->Indices.push_back(14);
    buffer->Indices.push_back(14); buffer->Indices.push_back(20); buffer->Indices.push_back(24);
    buffer->Indices.push_back(14); buffer->Indices.push_back(24); buffer->Indices.push_back(13);

    buffer->Indices.push_back(18); buffer->Indices.push_back(24); buffer->Indices.push_back(20);
    buffer->Indices.push_back(18); buffer->Indices.push_back(20); buffer->Indices.push_back(19);
    buffer->Indices.push_back(19); buffer->Indices.push_back(20); buffer->Indices.push_back(21);
    buffer->Indices.push_back(19); buffer->Indices.push_back(21); buffer->Indices.push_back(16);

    static_cast<scene::SAnimatedMesh*>(mesh)->recalculateBoundingBox();
    return mesh;
}


scene::IMesh* GeometryGenerator::createTracerMesh(f32 length, f32 height, u32 segments) const
{
    scene::IAnimatedMesh* mesh = new scene::SAnimatedMesh;
    scene::SMesh* tmpMesh = new scene::SMesh();
    scene::SMeshBuffer* buffer = new scene::SMeshBuffer();
    tmpMesh->addMeshBuffer(buffer);
    buffer->drop();
    static_cast<scene::SAnimatedMesh*>(mesh)->addMesh(tmpMesh);
    tmpMesh->drop();

    video::SColor white(255, 255, 255, 255);
    core::vector2df null2d(0, 0);
    f32 size = height / 2;

    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, size, -length), core::vector3df(0, 1, 0), white, null2d));
    buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, -size, -length), core::vector3df(0, -1, 0), white, null2d));

    for (u32  i = 1; i <= segments; i++)
    {
        buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, size, -length + (f32(i)/segments) * length), core::vector3df(0, 1, 0), white, null2d));
        buffer->Vertices.push_back(video::S3DVertex(core::vector3df(0, -size, -length + (f32(i)/segments) * length), core::vector3df(0, -1, 0), white, null2d));

        u32 Idx = i * 2;
        u32 prevIdx = (i-1) * 2;
        buffer->Indices.push_back(prevIdx); buffer->Indices.push_back(prevIdx+1); buffer->Indices.push_back(Idx);
        buffer->Indices.push_back(prevIdx+1); buffer->Indices.push_back(Idx+1); buffer->Indices.push_back(Idx);
    }

    static_cast<scene::SAnimatedMesh*>(mesh)->recalculateBoundingBox();
    return mesh;
}
