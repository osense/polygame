#ifndef SHADERCBCUBE_H
#define SHADERCBCUBE_H

#include "SContext.h"
#include <irrlicht.h>

using namespace irr;


class ShaderCBSolid : public video::IShaderConstantSetCallBack
{
    public:

        ShaderCBSolid(SContext* cont);

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        void OnSetMaterial(const video::SMaterial &material);

        void setTransform(f32 t);

        f32 getTransform() const;

    private:
        SContext* Context;
        bool FirstUpdate;

        video::SColorf Color;
        f32 ThicknessBias;
        f32 Transform;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 ColorID;
        s32 ThicknessBiasID;
        s32 TransformID;
};

#endif // SHADERCBCUBE_H
