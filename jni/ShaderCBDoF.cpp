#include "ShaderCBDoF.h"

void ShaderCBDoF::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        SceneID = services->getPixelShaderConstantID("Scene");
        DepthID = services->getPixelShaderConstantID("Depth");

        FirstUpdate = true;
    }

    s32 tex0= 0;
    s32 tex1= 1;
    services->setPixelShaderConstant(SceneID, &tex0, 1);
    services->setPixelShaderConstant(DepthID, &tex1, 1);
}
