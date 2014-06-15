#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "Object.h"
#include "ObjectManager.h"

#include <irrlicht.h>
#include <vector>

using namespace irr;

class ObjectGrid : public Object
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    void regenerate();


private:
    static const u32 NumPoints = 30;
    static const f32 LineThickness = 0.015;

    core::vector3df Position;
    f32 Points[NumPoints][NumPoints];


    scene::SMeshBuffer* Buffer;
    scene::IMeshSceneNode* Node;
    scene::SMeshBuffer* LineBuffer;


    void addPlusX();
    void addMinusX();
    void addPlusY();
    void addMinusY();

};

#endif // OBJECTGRID_H
