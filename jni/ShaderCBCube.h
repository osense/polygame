#ifndef SHADERCBCUBE_H
#define SHADERCBCUBE_H

#include "SContext.h"
#include <irrlicht.h>

using namespace irr;


class ShaderCBCube : public video::IShaderConstantSetCallBack
{
    public:

        ShaderCBCube(SContext* cont);

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        void setBaseColor(video::SColorf col);

        void setTransform(f32 t);

        f32 getTransform() const;

    private:
        SContext* Context;
        bool FirstUpdate;

        video::SColorf BaseColor;
        f32 Transform;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 BaseColorID;
        s32 TransformID;
};

#endif // SHADERCBCUBE_H
