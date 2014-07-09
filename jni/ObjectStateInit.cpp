#include "ObjectStateInit.h"

ObjectStateInit::ObjectStateInit(SContext* cont) : Object(cont)
{
    Name = "ObjectStateInit";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    // we'll need to parse saved settings here
    //Context->Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

    core::vector2d<s32> imgPos;
    video::ITexture* imgTex = Context->Device->getVideoDriver()->getTexture("gui/loading.png");
    imgPos.X = (Context->Device->getVideoDriver()->getScreenSize().Width / 2) - (imgTex->getSize().Width / 2);
    imgPos.Y = (Context->Device->getVideoDriver()->getScreenSize().Height / 2) - (imgTex->getSize().Height / 2);

    Image = Context->Device->getGUIEnvironment()->addImage(imgTex, imgPos);

    LoadingState = EILS_RENDERER;

    TextureNames.push_back("gui/continue.png");
    TextureNames.push_back("gui/new_game.png");
    TextureNames.push_back("gui/exit.png");
    TextureNames.push_back("gui/options.png");
}

ObjectStateInit::~ObjectStateInit()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateInit::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (LoadingState == EILS_RENDERER)
        {
            debugLog("precaching resources...");
            //Context->Renderer->init(EET_FXAA);
            //Context->Renderer->init(EET_DOF);

            LoadingState = EILS_SHADERS;
            return;
        }

        else if (LoadingState == EILS_SHADERS)
        {
            video::IGPUProgrammingServices* gpu = Context->Device->getVideoDriver()->getGPUProgrammingServices();
            Context->Mtls->Depth = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/depth.vert", "shaders/depth.frag",
                                   new ShaderCBDepth(Context));

            Context->Mtls->GridCB = new ShaderCBGrid(Context);
            Context->Mtls->Grid = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/grid.vert", "shaders/grid.frag",
                                  Context->Mtls->GridCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

            Context->Mtls->GridBack = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/grid_back.vert", "shaders/grid_back.frag",
                                      new ShaderCBGridBack(Context));

            Context->Mtls->Sky = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/sky.vert", "shaders/sky.frag",
                                 new ShaderCBSky());

            LoadingState = EILS_TEXTURES;
            TexturesLoaded = 0;
            return;
        }

        else if (LoadingState == EILS_TEXTURES)
        {
            Context->Device->getVideoDriver()->getTexture(TextureNames[TexturesLoaded++]);

            if (TexturesLoaded >= TextureNames.size())
                LoadingState = EILS_FONTS;

            return;
        }

        else if (LoadingState == EILS_FONTS)
        {
            Context->Device->getGUIEnvironment()->getFont("gui/asap.xml");

            LoadingState = EILS_DONE;
            return;
        }

        else if (LoadingState == EILS_DONE)
        {
            debugLog("done precaching resources");
            new ObjectStateMenu(Context);

            Image->remove();
            delete this;
        }
    }
}
