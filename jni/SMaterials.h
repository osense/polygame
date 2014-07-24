#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>

using namespace irr;

class ShaderCBGrid;
class ShaderCBGridBack;
class ShaderCBCube;

struct SMaterials
{
    video::E_MATERIAL_TYPE Depth;

    video::E_MATERIAL_TYPE Grid;
    ShaderCBGrid* GridCB;

    video::E_MATERIAL_TYPE GridBack;
    ShaderCBGridBack* GridBackCB;

    video::E_MATERIAL_TYPE ItemCube;
    ShaderCBCube* CubeCB;

    video::E_MATERIAL_TYPE Sky;
};


#endif // SMATERIALS_H_INCLUDED
