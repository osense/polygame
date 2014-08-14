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
        bool FirstUpdate;

        s32 WorldViewProjMatID;
        s32 TexID;
};

#endif // SHADERCBSKY_H
