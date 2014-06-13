#include "ShaderCBSimple.h"

ShaderCBSimple::ShaderCBSimple(SContext* cont)
    :Context(cont),
    FirstUpdate(false)
{

}

void ShaderCBSimple::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
    if(!FirstUpdate)
    {
        /*Tex0ID = services->getPixelShaderConstantID("Tex0");
        Tex1ID = services->getPixelShaderConstantID("Tex1");
        Tex2ID = services->getPixelShaderConstantID("Tex2");
        Light1ID = services->getPixelShaderConstantID("Light1");
        Light2ID = services->getPixelShaderConstantID("Light2");
        CamFarID = services->getVertexShaderConstantID("CamFar");
        WorldViewProjMatID = services->getVertexShaderConstantID("WorldViewProjMat");
        WorldViewMatID = services->getVertexShaderConstantID("WorldViewMat");*/
        ProjMatID = services->getVertexShaderConstantID("ProjMat");
        WorldMatID = services->getVertexShaderConstantID("WorldMat");
        ViewMatID = services->getVertexShaderConstantID("ViewMat");

        FirstUpdate = true;
    }

    /*s32 tex0= 0;
    s32 tex1= 1;
    s32 tex2= 2;
    services->setPixelShaderConstant(Tex0ID, &tex0, 1);
    services->setPixelShaderConstant(Tex1ID, &tex1, 1);
    services->setPixelShaderConstant(Tex2ID, &tex2, 1);*/

    core::matrix4 worldMat = services->getVideoDriver()->getTransform(video::ETS_WORLD);
    core::matrix4 viewMat = services->getVideoDriver()->getTransform(video::ETS_VIEW);
    core::matrix4 projMat = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
    /*core::matrix4 worldViewProj = projMat * viewMat * worldMat;
    core::matrix4 worldView = viewMat * worldMat;

    core::vector3df viewLight1;
    viewMat.transformVect(viewLight1, Light1);
    services->setPixelShaderConstant(Light1ID, (f32*)&viewLight1, 3);
    services->setPixelShaderConstant(Light2ID, (f32*)&Light2, 3);

    services->setVertexShaderConstant(WorldViewProjMatID, worldViewProj.pointer(), 16);
    services->setVertexShaderConstant(WorldViewMatID, worldView.pointer(), 16);*/
    services->setVertexShaderConstant(ProjMatID, projMat.pointer(), 16);
    services->setVertexShaderConstant(WorldMatID, worldMat.pointer(), 16);
    services->setVertexShaderConstant(ViewMatID, viewMat.pointer(), 16);

    /*f32 farDist = Context->Device->getSceneManager()->getActiveCamera()->getFarValue();
    services->setVertexShaderConstant(CamFarID, &farDist, 1);*/
}

void ShaderCBSimple::OnSetMaterial (const video::SMaterial &material)
{

}
