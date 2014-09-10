#ifndef SHADERCBSKY_H
#define SHADERCBSKY_H

#include <irrlicht.h>

using namespace irr;

class ShaderCBSky : public video::IShaderConstantSetCallBack
{
    public:
        ShaderCBSky();

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

    private:
        bool FirstUpdate = false;

        s32 WorldViewProjMatID = 0;
        s32 TexID = 0;
};

#endif // SHADERCBSKY_H
