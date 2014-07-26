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

#ifdef _IRR_ANDROID_PLATFORM_
    ANativeWindow* nativeWindow = static_cast<ANativeWindow*>(Context->Device->getVideoDriver()->getExposedVideoData().OGLESAndroid.Window);
    ScreenSize.Width = ANativeWindow_getWidth(Context->App->window);
    ScreenSize.Height = ANativeWindow_getHeight(Context->App->window);
#else
    ScreenSize = Context->Device->getVideoDriver()->getScreenSize();
#endif

    GUIHasEffects = false;
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
        if (GUIHasEffects)
            Context->Device->getGUIEnvironment()->drawAll();

        video->setRenderTarget(video::ERT_FRAME_BUFFER, false, false);

        PP->render(Scene);

        if (!GUIHasEffects)
            Context->Device->getGUIEnvironment()->drawAll();
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
        res = ScreenSize;

        core::stringc resText;
        resText += res.Width;
        resText += "x";
        resText += res.Height;
        Context->Device->getLogger()->log("Using Scene RTT with resolution", resText.c_str(), ELL_DEBUG);
        resText = "";
        resText += res.Width / (int)EffectQuality;
        resText += "x";
        resText += res.Height / (int)EffectQuality;
        Context->Device->getLogger()->log("Effect RTT resolution is", resText.c_str(), ELL_DEBUG);

        Scene = video->addRenderTargetTexture(res, "scene-RT");
        if (ScreenSize == res)
            GUIHasEffects = true;
        else
            GUIHasEffects = false;
        PP->setQuality(res / (u32)EffectQuality);
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
    {
        if (Glow)
            break;

        Context->Device->getLogger()->log("EffectRenderer", "initializing glow effect");
        Glow = PP->createEffectChain();
        Glow->createEffect(video::EPE_ALBEDO);
        Glow->createEffect(video::EPE_BLUR_V);
        video::CPostProcessingEffect* add = Glow->createEffect(video::EPE_BLUR_H_ADD);

        if (FXAA)
            add->addTextureToShader(FXAA->getCustomRTT());
        else
            add->addTextureToShader(Scene);

        break;
    }

    default:
        break;
    }

    Active = true;
}

core::dimension2du EffectRenderer::getScreenSize() const
{
    if (isActive() && GUIHasEffects)
        return Scene->getSize();
    else
        return ScreenSize;

}
