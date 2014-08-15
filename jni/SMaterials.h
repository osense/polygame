#ifndef SMATERIALS_H_INCLUDED
#define SMATERIALS_H_INCLUDED

#include <irrlicht.h>

using namespace irr;

class ShaderCBGrid;
class ShaderCBGridBack;
class ShaderCBSolid;
class ShaderCBFader;

struct SMaterials
{
    video::E_MATERIAL_TYPE Depth;

    video::E_MATERIAL_TYPE Grid;
    ShaderCBGrid* GridCB;

    video::E_MATERIAL_TYPE GridBack;
    ShaderCBGridBack* GridBackCB;

    video::E_MATERIAL_TYPE Solid;
    ShaderCBSolid* SolidCB;

    video::E_MATERIAL_TYPE Fader;
    ShaderCBFader* FaderCB;

    video::E_MATERIAL_TYPE Sky;

    bool Loaded;
};


#endif // SMATERIALS_H_INCLUDED
