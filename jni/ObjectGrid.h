#ifndef OBJECTGRID_H
#define OBJECTGRID_H

#include "ObjectManager.h"
#include "GridGenerator.h"
#include "SMaterials.h"
#include "ShaderCBGrid.h"
#include "ShaderCBGridBack.h"
#include "functions.h"

#include <stdlib.h>
#include <irrlicht.h>
#include <thread>
#include "circular_buffer.h"

using namespace irr;

class ObjectGrid : public Object
{
public:
    ObjectGrid(SContext* cont);
    ~ObjectGrid();
    virtual void onMessage(SMessage msg);

    void setCollision(bool active);

    core::vector3df getPosition() const;
    u32 getNumPointsZ() const;
    u32 getNumPointsX() const;

    f32 getBaseHeight(u32 z) const;
    f32 getHillHeight(u32 x, u32 z) const;

    const GridGenerator& getGenerator() const;


private:
    static constexpr u32 NumPointsZ = 18;
    static constexpr u32 NumPointsX = NumPointsZ * 2;//(16.0 / 9.0);
    static constexpr f32 LineThickness = 0.005;
    static constexpr u32 GenChangeEvery = 50;
    static constexpr u32 ColorChangeEvery = 35;
    static constexpr f32 PlayerSize = 0.05;

    core::vector3df Position;
    f32 Points[NumPointsZ][NumPointsX];
    circular_buffer<f32> BaseHeight;

    bool CollisionActive;

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
    void addZ();
    void addPlusX();
    void addMinusX();

    void removeXsegment();

    void handleGenUpdate();
    void handleColors();
    bool handleCollision(core::vector3df pPos, core::vector3df diffV);

};

#endif // OBJECTGRID_H
