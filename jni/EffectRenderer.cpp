#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    PP(0),
    Active(false),
    Scene(0),
    ForceFXAAOff(false),
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

    GUIHasEffects = true;

    Fader = new EffectFader(Context);
}

EffectRenderer::~EffectRenderer()
{
    delete Fader;
}

void EffectRenderer::drawAll(u32 timeDelta)
{
    if (isActive())
    {
        video::IVideoDriver* video = Context->Device->getVideoDriver();

        video->setRenderTarget(Scene);
        Smgr->drawAll();
        if (Fader->isActive() && !Fader->getIncludeGUI())
            Fader->draw(timeDelta / 1000.0);

        if (GUIHasEffects)
            Context->Device->getGUIEnvironment()->drawAll();

        video->setRenderTarget(video::ERT_FRAME_BUFFER);//, false, false);

        PP->render(Scene);

        if (!GUIHasEffects)
            Context->Device->getGUIEnvironment()->drawAll();

        if (Fader->isActive() && Fader->getIncludeGUI())
            Fader->draw(timeDelta / 1000.0);
    }
    else
    {
        Smgr->drawAll();

        if (Fader->isActive() && !Fader->getIncludeGUI())
            Fader->draw(timeDelta / 1000.0);

        Context->Device->getGUIEnvironment()->drawAll();

        if (Fader->isActive() && Fader->getIncludeGUI())
            Fader->draw(timeDelta / 1000.0);
    }
}

bool EffectRenderer::isActive() const
{
    return Active;
}


void EffectRenderer::init(E_EFFECT_TYPE type)
{
    video::IVideoDriver* video = Context->Device->getVideoDriver();

    if (type == EET_FADER)
    {
        Context->Device->getLogger()->log("EffectRenderer", "initializing fader effect");
        Fader->init();
        return;
    }

    if (!PP)
    {
        PP = createIrrPP(Context->Device, Context->Settings->EffectQuality, "shaders/pp/");

        core::dimension2d<u32> res(1024, 512);
        res = ScreenSize;

        core::stringc resText;

        if (!Scene)
        {
            resText += res.Width;
            resText += "x";
            resText += res.Height;
            Context->Device->getLogger()->log("Creating Scene RTT with resolution", resText.c_str(), ELL_DEBUG);
            Scene = video->addRenderTargetTexture(res, "scene-RT");
        }

        resText = "";
        resText += res.Width / (u32)Context->Settings->EffectQuality;
        resText += "x";
        resText += res.Height / (u32)Context->Settings->EffectQuality;
        Context->Device->getLogger()->log("Effect RTT resolution is", resText.c_str(), ELL_DEBUG);

        GUIHasEffects = (ScreenSize == res);

        PP->setQuality(res / (u32)Context->Settings->EffectQuality);
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
        video::CPostProcessingEffect* add = 0;

        if (Context->Settings->EffectQuality == video::EPQ_HALF)
        {
            Glow->createEffect(video::EPE_BLUR_V_HIGH);
            add = Glow->createEffect(video::EPE_BLUR_H_ADD_HIGH);
        }
        else if (Context->Settings->EffectQuality == video::EPQ_QUARTER)
        {
            Glow->createEffect(video::EPE_BLUR_V_MEDIUM);
            add = Glow->createEffect(video::EPE_BLUR_H_ADD_MEDIUM);
        }
        else
        {
            Glow->createEffect(video::EPE_BLUR_V_LOW);
            add = Glow->createEffect(video::EPE_BLUR_H_ADD_LOW);
        }


        if (FXAA)
            add->addTextureToShader(FXAA->getCustomRTT());
        else
            add->addTextureToShader(Scene);

        break;
    }

    default:
        return;
    }

    Active = true;
}

void EffectRenderer::loadPP(bool reload)
{
    if (reload)
    {
        if (PP)
            delete PP;
        PP = 0;
        FXAA = 0;
        Glow = 0;
        Active = false;
    }

    if (Context->Settings->Antialiasing)
        init(EET_FXAA);

    if (Context->Settings->Glow)
        init(EET_GLOW);

    setForceFXAAOff(ForceFXAAOff);

    if(PP)
        Context->Device->getLogger()->log("EffectRenderer", (core::stringc("Render pipeline setup:\n") + Context->Renderer->PP->getDebugString()).c_str());
    else
        Context->Device->getLogger()->log("EffectRenderer", "post-processing inactive");
}

void EffectRenderer::setForceFXAAOff(bool force)
{
    ForceFXAAOff = force;

    if (ForceFXAAOff && FXAA)
    {
        FXAA->setActive(false);
        GUIHasEffects = ScreenSize == Scene->getSize();
        if (Glow)
        {
            Glow->getEffectFromIndex(2)->removeTextureFromShader(0);
            Glow->getEffectFromIndex(2)->addTextureToShader(Scene);
        }
        else
            Active = false;
    }
    else if (!ForceFXAAOff && FXAA)
    {
        FXAA->setActive(true);
        GUIHasEffects = false;
        Active = true;
        if (Glow)
        {
            Glow->getEffectFromIndex(2)->removeTextureFromShader(0);
            Glow->getEffectFromIndex(2)->addTextureToShader(FXAA->getCustomRTT());
        }
    }
}

core::dimension2du EffectRenderer::getScreenSize() const
{
    if (isActive() && GUIHasEffects)
        return Scene->getSize();
    else
        return ScreenSize;

}

EffectFader* EffectRenderer::getFader() const
{
    return Fader;
}
