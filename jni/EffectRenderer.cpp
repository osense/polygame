#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    PP(0),
    Active(false),
    Scene(0),
    Glow(0)
{
    Context = cont;
    Smgr = Context->Device->getSceneManager();
    Video = Context->Device->getVideoDriver();

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
        Video->setRenderTarget(PP->getRTT1());
        Smgr->drawAll();

        Video->setRenderTarget(video::ERT_FRAME_BUFFER);
        Smgr->drawAll();

        if (Fader->isActive() && !Fader->getIncludeGUI())
            Fader->draw(timeDelta / 1000.0);

        PP->render(PP->getRTT1());
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
    if (type == EET_FADER)
    {
        Context->Device->getLogger()->log("EffectRenderer", "initializing fader effect");
        Fader->init();
        return;
    }

    if (!PP)
    {
        PP = createIrrPP(Context->Device, Context->Sets->EffectQuality, "shaders/pp/");
    }

    switch (type)
    {
    case EET_GLOW:
    {
        if (Glow)
            break;

        Context->Device->getLogger()->log("EffectRenderer", "initializing glow effect");
        Glow = PP->createEffectChain();

        if (Context->Sets->EffectQuality == video::EPQ_HALF)
        {
            Glow->createEffect(video::EPE_BLUR_V_HIGH);
            Glow->createEffect(video::EPE_BLUR_H_HIGH);
        }
        else if (Context->Sets->EffectQuality == video::EPQ_QUARTER)
        {
            Glow->createEffect(video::EPE_BLUR_V_MEDIUM);
            Glow->createEffect(video::EPE_BLUR_H_MEDIUM);
        }
        else
        {
            Glow->createEffect(video::EPE_BLUR_V_LOW);
            Glow->createEffect(video::EPE_BLUR_H_LOW);
        }


        Glow->createEffect(video::EPE_ALBEDO, video::EMT_TRANSPARENT_ADD_COLOR);

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
        Glow = 0;
        Active = false;
    }

    if (Context->Sets->Glow)
        init(EET_GLOW);

    if(PP)
        Context->Device->getLogger()->log("EffectRenderer", (core::stringc("Render pipeline setup:\n") + Context->Renderer->PP->getDebugString()).c_str());
    else
        Context->Device->getLogger()->log("EffectRenderer", "post-processing inactive");
}

EffectFader* EffectRenderer::getFader() const
{
    return Fader;
}
