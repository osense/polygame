#ifndef SHADERCBDOF_H
#define SHADERCBDOF_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;

class ShaderCBGrid : public video::IShaderConstantSetCallBack
{
public:

    ShaderCBGrid(SContext* cont);

    void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

    void setNearColor(video::SColorf n);

    void setFarColor(video::SColorf f);

    video::SColorf getNearColor() const;

    video::SColorf getFarColor() const;

    void setTransform(f32 t);

    f32 getTransform() const;

private:
    SContext* Context;
    bool FirstUpdate = false;

    video::SColorf NearColor, FarColor;
    f32 Transform = 0;

    s32 CamFarID = 0;
    s32 WorldViewProjMatID = 0, WorldViewMatID = 0;
    s32 NearColorID = 0, FarColorID = 0;
    s32 TransformID = 0;
};

#endif // SHADERCBDOF_H
