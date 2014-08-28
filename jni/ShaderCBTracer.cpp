#include "ShaderCBTracer.h"

ShaderCBTracer::ShaderCBTracer(SContext* cont)
{
    Context = cont;
    FirstUpdate = false;
    Transform = 0;
}

void ShaderCBTracer::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        ColorID = services->getVertexShaderConstantID("Color");
        TransformID = services->getVertexShaderConstantID("Transform");

        FirstUpdate = true;
    }

    core::matrix4 worldMat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    core::matrix4 viewMat = services->getVideoDriver()->getTransform(video::ETS_VIEW);
    core::matrix4 projMat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
    core::matrix4 worldView = viewMat * worldMat;
    core::matrix4 worldViewProj = projMat * worldView;

    services->setVertexShaderConstant(WorldViewProjMatID, worldViewProj.pointer(), 16);

    f32 farDist = Context->Device->getSceneManager()->getActiveCamera()->getFarValue();
    services->setVertexShaderConstant(CamFarID, &farDist, 1);

    services->setVertexShaderConstant(ColorID, reinterpret_cast<f32*>(&Color), 3);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBTracer::OnSetMaterial(const video::SMaterial &material)
{
    Color = video::SColorf(material.AmbientColor);
}

void ShaderCBTracer::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBTracer::getTransform() const
{
    return Transform;
}
