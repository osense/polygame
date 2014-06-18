#include "EffectRenderer.h"

EffectRenderer::EffectRenderer(SContext* cont) :
    Scene(0), Depth(0),
    DoFEnabled(false), DoFMaterial(video::EMT_SOLID) // = uninitialized
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

        if (DoFEnabled)
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
        }

        video->setRenderTarget(video::ERT_FRAME_BUFFER);

        Quad->render();

        #ifdef DEBUG_EFFECTS
        if (Depth)
            video->draw2DImage(Depth, core::position2d<s32>(0, 0));
        #endif
    }
    else
    {
        Smgr->drawAll();
    }
}

void EffectRenderer::setDoFEnabled(bool enable)
{
    if (enable && DoFMaterial == video::EMT_SOLID)
    {
        video::IGPUProgrammingServices* gpu = Context->Device->getVideoDriver()->getGPUProgrammingServices();
        DoFMaterial = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/quad.vert", "shaders/dof.frag", new ShaderCBDoF());

        initRT(EET_DOF);
    }

    DoFEnabled = enable;
}

bool EffectRenderer::isActive() const
{
    return DoFEnabled;
}


void EffectRenderer::initRT(E_EFFECT_TYPE type)
{
    video::IVideoDriver* video = Context->Device->getVideoDriver();
    core::dimension2d<u32> screenSize = video->getScreenSize();

    if (!Scene)
    {
        core::dimension2d<u32> res(1024, 512);// = screenSize / (1.0/SceneQuality);
        Scene = video->addRenderTargetTexture(res, "scene-RT", video::ECF_R5G6B5);

        Quad = new scene::IQuadSceneNode(0, EffectSmgr, -1);
        Quad->setMaterialTexture(0, Scene);
        Quad->setMaterialType(DoFMaterial);
    }

    switch (type)
    {
    case EET_DOF:
        core::dimension2d<u32> res = Scene->getOriginalSize() / (1.0/EffectQuality);
        Depth = video->addRenderTargetTexture(res, "depth-RT", video::ECF_R5G6B5);
        Quad->setMaterialTexture(1, Depth);
        break;
    }
}
