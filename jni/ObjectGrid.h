#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectVisual.h"
#include "ObjectManager.h"
#include "GridGenerator.h"
#include "SMaterials.h"

#include <irrlicht.h>

using namespace irr;

class ObjectGrid : public ObjectVisual
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    void regenerate();


private:
    static const u32 NumPoints = 25;
    static const f32 LineThickness = 0.005;

    core::vector3df Position;
    f32 Points[NumPoints][NumPoints];
    GridGenerator Generator;


    // lines geometry
    scene::SMeshBuffer* Buffer;
    // approximation mesh
    scene::SMeshBuffer* BufferAppx;
    scene::IMeshSceneNode* BackNode;


    void addPlusX();
    void addMinusX();
    void addPlusY();
    void addMinusY();

};

#endif // OBJECTGRID_H
