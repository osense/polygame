#ifndef CSHADERDEFAULTCALLBACK_H
#define CSHADERDEFAULTCALLBACK_H

#include <irrlicht.h>
#include "SContext.h"
#include "functions.h"

using namespace irr;


class ShaderCBGridBack : public video::IShaderConstantSetCallBack
{
    public:

        ShaderCBGridBack(SContext* cont);

        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        virtual void OnSetMaterial(const video::SMaterial &material);

        void setTransform(f32 t);

        f32 getTransform() const;

        void setAlpha(f32 a);

        f32 getAlpha() const;

    private:
        SContext* Context;
        bool FirstUpdate;

        video::SColorf NearColor, FarColor;
        f32 Transform;
        f32 Alpha;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 NearColorID, FarColorID;
        s32 TransformID;
        s32 AlphaID;
};

#endif // CSHADERDEFAULTCALLBACK_H
