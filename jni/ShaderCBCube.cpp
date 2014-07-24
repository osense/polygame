#include "ShaderCBCube.h"

ShaderCBCube::ShaderCBCube(SContext* cont)
{
    Context = cont;
    BaseColor = video::SColorf(1.0, 1.0, 1.0);
    Transform = 0;
}

void ShaderCBCube::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");
        BaseColorID = services->getVertexShaderConstantID("BaseColor");
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

    services->setVertexShaderConstant(BaseColorID, reinterpret_cast<f32*>(&BaseColor), 3);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBCube::setBaseColor(video::SColorf col)
{
    BaseColor = col;
}

void ShaderCBCube::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBCube::getTransform() const
{
    return Transform;
}
