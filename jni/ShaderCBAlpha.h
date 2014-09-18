#ifndef CSHADERDEFAULTCALLBACK_H
#define CSHADERDEFAULTCALLBACK_H

#include <irrlicht.h>
#include "SContext.h"
#include "functions.h"

using namespace irr;


class ShaderCBAlpha : public video::IShaderConstantSetCallBack
{
public:

    ShaderCBAlpha(SContext* cont);

    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

    virtual void OnSetMaterial(const video::SMaterial &material);

    void setTransform(f32 t);

    f32 getTransform() const;

private:
    SContext* Context;
    bool FirstUpdate = false;

    video::SColorf Color, ColorFar;
    f32 ThicknessBias = 0;
    f32 Transform = 0;

    s32 CamFarID = 0;
    s32 WorldViewProjMatID = 0, WorldViewMatID = 0;
    s32 ColorID = 0, ColorFarID = 0;
    s32 ThicknessBiasID = 0;
    s32 TransformID = 0;
};

#endif // CSHADERDEFAULTCALLBACK_H
