#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>

using namespace irr;

class ShaderCBGrid;

struct SMaterials
{
    video::E_MATERIAL_TYPE Depth;
    video::E_MATERIAL_TYPE Grid;
    ShaderCBGrid* GridCB;
    video::E_MATERIAL_TYPE GridBack;
    video::E_MATERIAL_TYPE ColorBlend;
};


#endif // SMATERIALS_H_INCLUDED
