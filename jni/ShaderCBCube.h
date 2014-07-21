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

    private:
        SContext* Context;
        bool FirstUpdate;

        video::SColorf BaseColor;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 BaseColorID;
};

#endif // SHADERCBCUBE_H
