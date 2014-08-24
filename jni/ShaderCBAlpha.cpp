#include "ShaderCBAlpha.h"

ShaderCBAlpha::ShaderCBAlpha(SContext* cont)
    :FirstUpdate(false)
{
    Context = cont;
    Transform = 0;
    Alpha = 1;
}

void ShaderCBAlpha::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");
        AlphaID = services->getVertexShaderConstantID("Alpha");
        ColorID = services->getVertexShaderConstantID("Color");
        ColorFarID = services->getVertexShaderConstantID("ColorFar");
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

    services->setVertexShaderConstant(AlphaID, &Alpha, 1);

    services->setVertexShaderConstant(ColorID, reinterpret_cast<f32*>(&Color), 3);
    services->setVertexShaderConstant(ColorFarID, reinterpret_cast<f32*>(&ColorFar), 3);

    services->setVertexShaderConstant(ThicknessBiasID, &ThicknessBias, 1);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBAlpha::OnSetMaterial (const video::SMaterial &material)
{
    Alpha = 1 - material.MaterialTypeParam;
    Color = video::SColorf(material.AmbientColor);
    ColorFar = video::SColorf(material.DiffuseColor);
    ThicknessBias = material.Thickness - 1;
}

void ShaderCBAlpha::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBAlpha::getTransform() const
{
    return Transform;
}
