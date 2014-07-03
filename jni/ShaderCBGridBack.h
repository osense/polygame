#ifndef CSHADERDEFAULTCALLBACK_H
#define CSHADERDEFAULTCALLBACK_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;


class ShaderCBGridBack : public video::IShaderConstantSetCallBack
{
    public:

        ShaderCBGridBack(SContext* cont);

        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        virtual void OnSetMaterial(const video::SMaterial &material);

    private:
        SContext* Context;
        bool FirstUpdate;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 NearColorID, FarColorID;
};

#endif // CSHADERDEFAULTCALLBACK_H
