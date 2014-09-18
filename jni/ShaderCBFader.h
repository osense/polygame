#ifndef SHADERCBFADER_H
#define SHADERCBFADER_H

#include <irrlicht.h>

using namespace irr;

class ShaderCBFader : public video::IShaderConstantSetCallBack
{
public:
    ShaderCBFader();

    void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

    void setAlpha(f32 alph);

    f32 getAlpha() const;

private:
    bool FirstUpdate = false;

    f32 Alpha = 0;

    s32 AlphaID = 0;
};

#endif // SHADERCBFADER_H
