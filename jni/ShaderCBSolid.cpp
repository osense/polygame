#include "ShaderCBSolid.h"

ShaderCBSolid::ShaderCBSolid(SContext* cont)
    :Context(cont)
{
    Color = video::SColorf(1.0, 1.0, 1.0);
}

void ShaderCBSolid::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");
        ColorID = services->getVertexShaderConstantID("Color");
        ThicknessBiasID = services->getVertexShaderConstantID("ThicknessBias");
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

    services->setVertexShaderConstant(ColorID, reinterpret_cast<f32*>(&Color), 3);

    services->setVertexShaderConstant(ThicknessBiasID, &ThicknessBias, 1);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBSolid::OnSetMaterial(const video::SMaterial &material)
{
    Color = video::SColorf(material.AmbientColor);
    ThicknessBias = material.Thickness - 1;
}

void ShaderCBSolid::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBSolid::getTransform() const
{
    return Transform;
}
