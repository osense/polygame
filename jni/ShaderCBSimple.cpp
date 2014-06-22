#include "ShaderCBSimple.h"

void ShaderCBSimple::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");

        FirstUpdate = true;
    }

    core::matrix4 worldMat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    core::matrix4 viewMat = services->getVideoDriver()->getTransform(video::ETS_VIEW);
    core::matrix4 projMat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
    core::matrix4 worldViewProj = projMat * viewMat * worldMat;

    services->setVertexShaderConstant(WorldViewProjMatID, worldViewProj.pointer(), 16);
}

void ShaderCBSimple::OnSetMaterial (const video::SMaterial &material)
{

}
