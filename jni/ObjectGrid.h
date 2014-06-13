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


private:
    LinePool Pool;
};

#endif // OBJECTGRID_H
