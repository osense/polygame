#include "ShaderCBFader.h"

ShaderCBFader::ShaderCBFader()
    :FirstUpdate(false),
    Alpha(0)
{

}

void ShaderCBFader::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        AlphaID = services->getVertexShaderConstantID("Alpha");

        FirstUpdate = true;
    }

    services->setPixelShaderConstant(AlphaID, &Alpha, 1);
}


void ShaderCBFader::setAlpha(f32 alph)
{
    Alpha = alph;
}

f32 ShaderCBFader::getAlpha() const
{
    return Alpha;
}
