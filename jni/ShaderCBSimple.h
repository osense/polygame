#ifndef CSHADERDEFAULTCALLBACK_H
#define CSHADERDEFAULTCALLBACK_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;


class ShaderCBSimple : public video::IShaderConstantSetCallBack
{
    public:
        ShaderCBSimple(SContext* cont);

        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        virtual void OnSetMaterial(const video::SMaterial &material);

    private:
        SContext* Context;
        bool FirstUpdate;

        //s32 Tex0ID, Tex1ID, Tex2ID;
        //s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID, ProjMatID, WorldMatID, ViewMatID;
};

#endif // CSHADERDEFAULTCALLBACK_H
