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
        bool FirstUpdate;

        f32 Alpha;
        video::SColorf Color, ColorFar;
        f32 ThicknessBias;
        f32 Transform;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 ColorID, ColorFarID;
        s32 ThicknessBiasID;
        s32 TransformID;
        s32 AlphaID;
};

#endif // CSHADERDEFAULTCALLBACK_H
