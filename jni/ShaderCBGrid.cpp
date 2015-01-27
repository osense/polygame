#include "ShaderCBGrid.h"

ShaderCBGrid::ShaderCBGrid(SContext* cont)
    :Context(cont)
{
    NearColor = video::SColorf(1, 1, 1);
    FarColor = video::SColorf(1, 1, 1);
}

void ShaderCBGrid::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        CamFarID = services->getVertexShaderConstantID("CamFar");
        CamPosID = services->getVertexShaderConstantID("CamPos");
        //WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");
        WorldMatID = services->getVertexShaderConstantID("WorldMat");
        ViewMatID = services->getVertexShaderConstantID("ViewMat");
        ProjMatID = services->getVertexShaderConstantID("ProjdMat");
        ViewProjMatID = services->getVertexShaderConstantID("ViewProjMat");
        NearColorID = services->getVertexShaderConstantID("NearColor");
        FarColorID = services->getVertexShaderConstantID("FarColor");
        TransformID = services->getVertexShaderConstantID("Transform");

        FirstUpdate = true;
    }

    core::matrix4 worldMat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    core::matrix4 viewMat = services->getVideoDriver()->getTransform(video::ETS_VIEW);
    core::matrix4 projMat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
    core::matrix4 worldView = viewMat * worldMat;
    //core::matrix4 worldViewProj = projMat * worldView;

    //services->setVertexShaderConstant(WorldViewProjMatID, worldViewProj.pointer(), 16);
    services->setVertexShaderConstant(WorldViewMatID, worldView.pointer(), 16);
    services->setVertexShaderConstant(WorldMatID, worldMat.pointer(), 16);
    services->setVertexShaderConstant(ViewMatID, viewMat.pointer(), 16);
    services->setVertexShaderConstant(ProjMatID, projMat.pointer(), 16);
    services->setVertexShaderConstant(ViewProjMatID, (projMat * viewMat).pointer(), 16);

    f32 farDist = Context->Device->getSceneManager()->getActiveCamera()->getFarValue();
    services->setVertexShaderConstant(CamFarID, &farDist, 1);
    f32 camPos[3];
    Context->Device->getSceneManager()->getActiveCamera()->getPosition().getAs3Values(reinterpret_cast<f32*>(&camPos));
    services->setVertexShaderConstant(CamPosID, reinterpret_cast<f32*>(&camPos), 3);

    services->setVertexShaderConstant(NearColorID, reinterpret_cast<f32*>(&NearColor), 3);
    services->setVertexShaderConstant(FarColorID, reinterpret_cast<f32*>(&FarColor), 3);

    services->setVertexShaderConstant(TransformID, &Transform, 1);
}

void ShaderCBGrid::setNearColor(video::SColorf n)
{
    NearColor = n;
}

void ShaderCBGrid::setFarColor(video::SColorf f)
{
    FarColor = f;
}

video::SColorf ShaderCBGrid::getNearColor() const
{
    return NearColor;
}

video::SColorf ShaderCBGrid::getFarColor() const
{
    return FarColor;
}

void ShaderCBGrid::setTransform(f32 t)
{
    Transform = t;
}

f32 ShaderCBGrid::getTransform() const
{
    return Transform;
}
