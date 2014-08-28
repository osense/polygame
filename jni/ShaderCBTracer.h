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
        bool FirstUpdate;

        video::SColorf Color;
        f32 Transform;

        s32 ColorID;
        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 TransformID;
};

#endif // SHADERCBDEPTH_H
