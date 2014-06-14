#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "Object.h"
#include "ObjectManager.h"

#include "LinePool.h"
#include <irrlicht.h>

// uncomment to enable debugging of the grid
#define DEBUG_GRID

using namespace irr;

class ObjectGrid : public Object
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    void regenerate();


private:
    static const u32 NumPoints = 25;
    static const f32 LineThickness = 0.015;

    core::matrix4 Translation;
    f32 Points[NumPoints][NumPoints];

    scene::SMeshBuffer* Buffer;
    scene::IMeshSceneNode* Node;
    scene::SMeshBuffer* LineBuffer;

};

#endif // OBJECTGRID_H
