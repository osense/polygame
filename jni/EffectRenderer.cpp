#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    PP(0),
    Active(false),
    Scene(0),
    FXAA(0),
    Glow(0)
{
    Context = cont;
    Smgr = Context->Device->getSceneManager();
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
        PP = createIrrPP(Context->Device, EffectQuality, "shaders/pp/");

        core::dimension2d<u32> res(1024, 512);
        core::dimension2d<u32> screenSize = video->getScreenSize();
        //res.Width = core::round32(screenSize.Width * SceneQuality);
        //res.Height = core::round32(screenSize.Height * SceneQuality);
        Scene = video->addRenderTargetTexture(res, "scene-RT");
    }

    switch (type)
    {
    case EET_FXAA:
        if (FXAA)
            break;

        Context->Device->getLogger()->log("EffectRenderer", "initializing fxaa");
        FXAA = PP->createEffect(video::EPE_FXAA);
        FXAA->setQuality(Scene->getSize());
        break;

    case EET_GLOW:
        if (Glow)
            break;

        Context->Device->getLogger()->log("EffectRenderer", "initializing glow effect");
        Glow = PP->createEffectChain();
        Glow->createEffect(PP->getRootEffectChain()->readShader("blur_select.frag"));
        Glow->createEffect(video::EPE_BLUR_H);
        Glow->createEffect(video::EPE_BLUR_V);
        video::CPostProcessingEffect* add2 = Glow->createEffect(video::EPE_ADD2);
        if (FXAA)
            add2->addTextureToShader(FXAA->getCustomRTT());
        else
            add2->addTextureToShader(Scene);

        break;
    }

    Active = true;
}
