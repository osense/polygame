#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    PP(0),
    Scene(0), Depth(0),
    FXAA(0),
    DoF(0)
{
    Context = cont;

    Smgr = Context->Device->getSceneManager();
    EffectSmgr = Smgr->createNewSceneManager();
    Camera = EffectSmgr->addCameraSceneNode();
}

EffectRenderer::~EffectRenderer()
{

}

void EffectRenderer::drawAll()
{
    if (isActive())
    {
        video::IVideoDriver* video = Context->Device->getVideoDriver();

        video->setRenderTarget(Scene);
        Smgr->drawAll();

        /*if (DoF)
        {
            video->setRenderTarget(Depth);

            scene::ICameraSceneNode* activeCam = Smgr->getActiveCamera();
            Camera->setPosition(activeCam->getPosition());
            Camera->setTarget(activeCam->getTarget());

            video->getOverrideMaterial().EnablePasses = scene::ESNRP_SOLID;
            video->getOverrideMaterial().EnableFlags = video::EMF_MATERIAL_TYPE;
            video->getOverrideMaterial().Material.MaterialType = Context->Mtls->Depth;

            Smgr->drawAll();

            video->getOverrideMaterial().EnablePasses = 0;
        }*/

        video->setRenderTarget(video::ERT_FRAME_BUFFER);

        PP->render(Scene);

        /*#ifdef DEBUG_EFFECTS
        if (Depth)
            video->draw2DImage(Depth, core::position2d<s32>(0, 0));
        #endif*/
    }
    else
    {
        Smgr->drawAll();
    }
}

bool EffectRenderer::isActive() const
{
    return (PP != 0);
}


void EffectRenderer::init(E_EFFECT_TYPE type)
{
    video::IVideoDriver* video = Context->Device->getVideoDriver();
    core::dimension2d<u32> screenSize = video->getScreenSize();

    if (!PP)
    {
        PP = createIrrPP(Context->Device, EffectQuality, "shaders/pp/");

        core::dimension2d<u32> res(1024, 512);
        //res.Width = core::round32(screenSize.Width * SceneQuality);
        //res.Height = core::round32(screenSize.Height * SceneQuality);
        Scene = video->addRenderTargetTexture(res, "scene-RT", video::ECF_R5G6B5);
    }

    switch (type)
    {
    case EET_FXAA:
        FXAA = PP->createEffect(video::EPE_FXAA);
        FXAA->setQuality(video::EPQ_FULL);
        break;

    case EET_DOF:
        DoF = PP->createEffectChain();
        DoF->createEffect(video::EPE_BLUR_V);
        DoF->createEffect(video::EPE_BLUR_H);
        video::CPostProcessingEffect* add2 = DoF->createEffect(video::EPE_ADD2);
        if (FXAA)
            add2->addTextureToShader(FXAA->getCustomRTT());
        else
            add2->addTextureToShader(Scene);

        /*core::dimension2d<u32> res = Scene->getOriginalSize() / (u32)EffectQuality;
        Depth = video->addRenderTargetTexture(res, "depth-RT", video::ECF_R5G6B5);*/
        break;
    }
}
