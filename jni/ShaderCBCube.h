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

    private:
        SContext* Context;
        bool FirstUpdate;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
};

#endif // SHADERCBCUBE_H
