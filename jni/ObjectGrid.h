#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectVisual.h"
#include "ObjectManager.h"
#include "GridGenerator.h"
#include "SMaterials.h"

#include <irrlicht.h>

using namespace irr;

class ObjectGrid : public Object
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    void regenerate();


private:
    static const u32 NumPointsY = 15;
    static const u32 NumPointsX = NumPointsY * (16.0 / 9.0);
    static const f32 LineThickness = 0.005;

    core::vector3df Position;
    f32 Points[NumPointsX][NumPointsY];
    GridGenerator Generator;

    // lines geometry
    scene::SMeshBuffer* Buffer;
    scene::IMeshSceneNode* Node;
    // approximation mesh
    scene::SMeshBuffer* BufferAppx;
    scene::IMeshSceneNode* BackNode;


    void addX();
    void addPlusY();
    void addMinusY();

};

#endif // OBJECTGRID_H
