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
        bool FirstUpdate;

        f32 Alpha;

        s32 AlphaID;
};

#endif // SHADERCBFADER_H
