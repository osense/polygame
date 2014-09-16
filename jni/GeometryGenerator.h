#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <irrlicht.h>

using namespace irr;

class GeometryGenerator
{
    public:

        static scene::IMesh* createLineMeshZ(f32 size);

        static scene::IMesh* createLineMeshX(f32 size);

        static scene::IMesh* createCubeMesh(f32 size, bool filled = false);

        static scene::IMesh* createPyramidMesh(f32 size, bool filled = false);

        static scene::IMesh* createTracerMesh(f32 length, f32 height, u32 segments);
};

#endif // GEOMETRYGENERATOR_H
