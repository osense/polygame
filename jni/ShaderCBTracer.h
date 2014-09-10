#ifndef SHADERCBDEPTH_H
#define SHADERCBDEPTH_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;

class ShaderCBTracer : public video::IShaderConstantSetCallBack
{
    public:
        ShaderCBTracer(SContext* cont);

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        void OnSetMaterial(const video::SMaterial &material);

        void setTransform(f32 t);

        f32 getTransform() const;

    private:
        SContext* Context;
        bool FirstUpdate = false;

        video::SColorf Color;
        f32 Transform = 0;

        s32 ColorID = 0;
        s32 CamFarID = 0;
        s32 WorldViewProjMatID = 0, WorldViewMatID = 0;
        s32 TransformID = 0;
};

#endif // SHADERCBDEPTH_H
