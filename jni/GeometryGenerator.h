#ifndef GEOMETRYGENERATOR_H
#define GEOMETRYGENERATOR_H

#include <irrlicht.h>

using namespace irr;

class GeometryGenerator
{
    public:

        scene::IMesh* createLineMesh(f32 size) const;

        scene::IMesh* createCubeMesh(f32 size, bool filled = false) const;

        scene::IMesh* createPyramidMesh(f32 size, bool filled = false) const;
};

#endif // GEOMETRYGENERATOR_H
