#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectManager.h"
#include "GridGenerator.h"
#include "SMaterials.h"
#include "ShaderCBGrid.h"
#include "functions.h"

#include <stdlib.h>
#include <irrlicht.h>

using namespace irr;

class ObjectGrid : public Object
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    const GridGenerator& getGenerator() const;


private:
    static const u32 NumPointsY = 18;
    static const u32 NumPointsX = NumPointsY * (16.0 / 9.0);
    static const f32 LineThickness = 0.005;
    static const u32 GenChangeEvery = 150;
    static const u32 ColorChangeEvery = 35;
    static const f32 PlayerSize = 0.05;

    core::vector3df Position;
    f32 Points[NumPointsX][NumPointsY];

    GridGenerator Generator;
    u32 GenChangeIn;

    u32 ColorChangeIn;
    u32 ChangingColor;
    video::SColorf ColorFar, ColorNext;

    // lines geometry
    scene::SMeshBuffer* Buffer;
    scene::IMeshSceneNode* Node;
    // approximation mesh
    scene::SMeshBuffer* BufferAppx;
    scene::IMeshSceneNode* BackNode;


    void regenerate();
    void addX();
    void addPlusY();
    void addMinusY();

    void handleGenUpdate();
    void handleColors();
    bool handleCollision(core::vector3df pPos, core::vector3df diffV);

};

#endif // OBJECTGRID_H
