#include "ShaderCBGridBack.h"

ShaderCBGridBack::ShaderCBGridBack(SContext* cont)
{
    Context = cont;
    Transform = 0;
}

void ShaderCBGridBack::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");
        TransformID = services->getVertexShaderConstantID("Transform");

        FirstUpdate = true;
    }

    core::matrix4 worldMat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    core::matrix4 viewMat = services->getVideoDriver()->getTransform(video::ETS_VIEW);
    core::matrix4 projMat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
    core::matrix4 worldView = viewMat * worldMat;
    core::matrix4 worldViewProj = projMat * worldView;

    services->setVertexShaderConstant(WorldViewProjMatID, worldViewProj.pointer(), 16);
    services->setVertexShaderConstant(WorldViewMatID, worldView.pointer(), 16);

    f32 farDist = Context->Device->getSceneManager()->getActiveCamera()->getFarValue();
    services->setVertexShaderConstant(CamFarID, &farDist, 1);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBGridBack::OnSetMaterial (const video::SMaterial &material)
{

}

void ShaderCBGridBack::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBGridBack::getTransform() const
{
    return Transform;
}
