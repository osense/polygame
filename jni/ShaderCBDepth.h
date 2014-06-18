#ifndef SHADERCBDEPTH_H
#define SHADERCBDEPTH_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;

class ShaderCBDepth : public video::IShaderConstantSetCallBack
{
    public:
        ShaderCBDepth(SContext* cont);

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        void OnSetMaterial(const video::SMaterial &material);

    private:
        SContext* Context;
        bool FirstUpdate;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
};

#endif // SHADERCBDEPTH_H
