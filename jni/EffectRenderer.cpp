#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    PP(0),
    Active(false),
    Scene(0), Depth(0),
    FXAA(0),
    DoF(0)
{
    Context = cont;
    Smgr = Context->Device->getSceneManager();

    PP = createIrrPP(Context->Device, EffectQuality, "shaders/pp/");

    /*video::IVideoDriver* video = Context->Device->getVideoDriver();
    core::dimension2d<u32> res(1024, 512);
    core::dimension2d<u32> screenSize = video->getScreenSize();
    res.Width = core::round32(screenSize.Width * SceneQuality);
    res.Height = core::round32(screenSize.Height * SceneQuality);
    CrashEffectTexture = video->addRenderTargetTexture(res, "CrashEffect-RT", video::ECF_R5G6B5);
    video::CPostProcessingEffect* crashRender = PP->createEffect(PP->getRootEffectChain()->readShader("crash_effect.frag"));
    PP->render(0, CrashEffectTexture);
    PP->getRootEffectChain()->removeEffect(crashRender);*/


    /*EffectSmgr = Smgr->createNewSceneManager();
    Camera = EffectSmgr->addCameraSceneNode();*/
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
    }
    else
    {
        Smgr->drawAll();
    }
}

bool EffectRenderer::isActive() const
{
    return Active;
}


void EffectRenderer::init(E_EFFECT_TYPE type)
{
    video::IVideoDriver* video = Context->Device->getVideoDriver();

    if (!Scene)
    {
        core::dimension2d<u32> res(1024, 512);
        core::dimension2d<u32> screenSize = video->getScreenSize();
        //res.Width = core::round32(screenSize.Width * SceneQuality);
        //res.Height = core::round32(screenSize.Height * SceneQuality);
        Scene = video->addRenderTargetTexture(res, "scene-RT");
    }

    switch (type)
    {
    case EET_FXAA:
        FXAA = PP->createEffect(video::EPE_FXAA);
        FXAA->setQuality(Scene->getSize());
        break;

    case EET_DOF:
        DoF = PP->createEffectChain();
        DoF->createEffect(video::EPE_ALBEDO);
        DoF->createEffect(video::EPE_BLUR_H);
        DoF->createEffect(video::EPE_BLUR_V);
        //DoF->createEffect(video::EPE_ALBEDO);
        video::CPostProcessingEffect* add2 = DoF->createEffect(video::EPE_ADD2);
        if (FXAA)
            add2->addTextureToShader(FXAA->getCustomRTT());
        else
            add2->addTextureToShader(Scene);

        break;
    }

    Active = true;
}


video::ITexture* EffectRenderer::getCrashEffectTexture() const
{
    return CrashEffectTexture;
}
