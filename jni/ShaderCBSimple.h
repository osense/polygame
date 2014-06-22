#ifndef CSHADERDEFAULTCALLBACK_H
#define CSHADERDEFAULTCALLBACK_H

#include <irrlicht.h>
#include "SContext.h"

using namespace irr;


class ShaderCBSimple : public video::IShaderConstantSetCallBack
{
    public:
        virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        virtual void OnSetMaterial(const video::SMaterial &material);

    private:
        bool FirstUpdate;

        s32 WorldViewProjMatID;
};

#endif // CSHADERDEFAULTCALLBACK_H
