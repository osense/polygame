#include "ObjectStateInit.h"

ObjectStateInit::ObjectStateInit(SContext* cont, bool showLoading) : Object(cont),
    Loading(0)
{
    Name = "ObjectStateInit";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    Context->Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);


    if (showLoading)
    {
        Loading = Context->Device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(0, 0, 1, 1));
        Loading->setRelativePositionProportional(core::rect<f32>(0, 0, 1, 1));
        Loading->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    }

    LoadingState = EILS_WAIT;
    WaitCounter = 3;

    TextureNames.push_back("textures/noise.png");
    TextureNames.push_back("textures/line_v.png");

    debugLog("LOADING...");
}

ObjectStateInit::~ObjectStateInit()
{
    debugLog("DONE");
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectStateInit::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (LoadingState == EILS_WAIT)
        {
            if (--WaitCounter <= 0)
                LoadingState = EILS_SETTINGS;
        }

        else if (LoadingState == EILS_SETTINGS)
        {
            // setting determined on the fly
            core::dimension2du screenSize = Context->Renderer->getScreenSize();
            f32 gScaleX = screenSize.Width / float(GUI_TARGET_X);
            f32 gScaleY = screenSize.Height / float(GUI_TARGET_Y);
            Context->GUIScale = core::vector2df(gScaleX, gScaleY);
            debugLog((core::stringc("GUI Scale is ") + core::stringc(Context->GUIScale.X) + "x" + core::stringc(Context->GUIScale.Y)).c_str());

            Context->ScreenSize = ESS_SMALL;
            if (screenSize.Width >= 960 && screenSize.Height >= 720)
                Context->ScreenSize = ESS_XLARGE;
            else if (screenSize.Width >= 640 && screenSize.Height >= 480)
                Context->ScreenSize = ESS_LARGE;
            else if (screenSize.Width >= 470 && screenSize.Height >= 320)
                Context->ScreenSize = ESS_NORMAL;

            debugLog(core::stringc("Screen size is ") + core::stringc(Context->ScreenSize));

            // saved settings
            Context->Settings = new SSettings;

#ifdef _IRR_ANDROID_PLATFORM_
            Context->Settings->FilePath = (core::stringc(Context->App->activity->internalDataPath) + "/") + SETTINGS_FILENAME;
            Context->SavegamePath = (core::stringc(Context->App->activity->internalDataPath) + "/") + "savegame.json";
            Context->SavegameInfoPath = (core::stringc(Context->App->activity->internalDataPath) + "/") + "savegame_info.json";
#else
            Context->Settings->FilePath = SETTINGS_FILENAME;
            Context->SavegamePath = "savegame.json";
            Context->SavegameInfoPath = "savegame_info.json";
#endif


            if (!loadSettings(Context))
            {
                debugLog("could not load saved settings, using default");
                initDefaultSettings(Context->Settings);
                writeSettings(Context);
            }

            LoadingState = EILS_FONTS;
        }

        else if (LoadingState == EILS_FONTS)
        {
            Context->Device->getGUIEnvironment()->getSkin()->setFont(getFont(Context));
            getOverlayFont(Context);
            Context->Device->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255, 255, 255, 255));
            Context->Device->getGUIEnvironment()->getSkin()->setColor(gui::EGDC_GRAY_TEXT, video::SColor(255, 50, 50, 50));
            Context->Device->getGUIEnvironment()->getSkin()->setSize(gui::EGDS_BUTTON_PRESSED_TEXT_OFFSET_X, 0);
            Context->Device->getGUIEnvironment()->getSkin()->setSize(gui::EGDS_BUTTON_PRESSED_TEXT_OFFSET_Y, 0);

            LoadingState = EILS_RENDERER;
            return;
        }

        else if (LoadingState == EILS_RENDERER)
        {
            Context->Renderer->loadPP();

            LoadingState = EILS_SHADERS;
            return;
        }

        else if (LoadingState == EILS_SHADERS)
        {
            if (!Context->Mtls->Loaded)
            {
                debugLog("loading shaders...");

                video::IGPUProgrammingServices* gpu = Context->Device->getVideoDriver()->getGPUProgrammingServices();
                /*Context->Mtls->Depth = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/depth.vert", "shaders/depth.frag",
                                                                                                        new ShaderCBDepth(Context));*/

                Context->Mtls->GridCB = new ShaderCBGrid(Context);
                Context->Mtls->Grid = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/grid.vert", "shaders/grid.frag",
                                                                                                        Context->Mtls->GridCB);

                Context->Mtls->GridBackCB = new ShaderCBGridBack(Context);
                Context->Mtls->GridBack = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/grid_back.vert", "shaders/grid_back.frag",
                                                                                                            Context->Mtls->GridBackCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

                Context->Mtls->CubeCB = new ShaderCBCube(Context);
                Context->Mtls->ItemCube = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/cube.vert", "shaders/cube.frag",
                                                                                                            Context->Mtls->CubeCB);

                Context->Mtls->FaderCB = new ShaderCBFader();
                Context->Mtls->Fader = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/fader.vert", "shaders/fader.frag",
                                                                                                            Context->Mtls->FaderCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

                Context->Mtls->Sky = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/sky.vert", "shaders/sky.frag",
                                                                                                    new ShaderCBSky());

                Context->Renderer->init(EET_FADER);

                Context->Mtls->Loaded = true;
            }

            LoadingState = EILS_MESHES;
            TexturesLoaded = 0;
            return;
        }

        else if (LoadingState == EILS_MESHES)
        {
            GeometryGenerator geomGen;

            scene::IAnimatedMesh* cube_mesh = static_cast<scene::IAnimatedMesh*>(geomGen.createCubeMesh(ObjectItemCube::getCubeSize()));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh", cube_mesh);
            cube_mesh->drop();

            scene::IAnimatedMesh* cube_mesh_filled = static_cast<scene::IAnimatedMesh*>(geomGen.createCubeMesh(ObjectItemCube::getCubeSize(), true));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh-filled", cube_mesh_filled);
            cube_mesh_filled->drop();

            LoadingState = EILS_TEXTURES;
        }

        else if (LoadingState == EILS_TEXTURES)
        {
            Context->Device->getVideoDriver()->getTexture(TextureNames[TexturesLoaded++]);

            if (TexturesLoaded >= TextureNames.size())
                LoadingState = EILS_DONE;

            return;
        }

        else if (LoadingState == EILS_DONE)
        {
            new ObjectStateMenu(Context);

            if (Loading)
                Loading->remove();

            delete this;
        }
    }
}
