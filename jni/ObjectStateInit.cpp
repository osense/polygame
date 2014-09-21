#include "ObjectStateInit.h"

ObjectStateInit::ObjectStateInit(SContext* cont, bool showLoading) : Object(cont),
    Loading(0)
{
    Name = "ObjectStateInit";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    Context->Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);

#ifdef _IRR_ANDROID_PLATFORM_
    ANativeWindow* nativeWindow = static_cast<ANativeWindow*>(Context->Device->getVideoDriver()->getExposedVideoData().OGLESAndroid.Window);
    Context->ScreenResolution.Width = ANativeWindow_getWidth(Context->App->window);
    Context->ScreenResolution.Height = ANativeWindow_getHeight(Context->App->window);
#else
    Context->ScreenResolution = Context->Device->getVideoDriver()->getScreenSize();
#endif


    if (showLoading)
    {
        Loading = Context->Device->getGUIEnvironment()->addStaticText(L"", core::rect<s32>(0, 0, 1, 1));
        Loading->setRelativePositionProportional(core::rect<f32>(0, 0, 1, 1));
        Loading->setTextAlignment(gui::EGUIA_CENTER, gui::EGUIA_CENTER);
    }

    LoadingState = EILS_WAIT;
    WaitCounter = 3;

    TexturesLoaded = 0;
    TextureNames.push_back("textures/noise.png");
    TextureNames.push_back("textures/line_v.png");
    TextureNames.push_back("textures/line_h.png");
    TextureNames.push_back("textures/line_v_bright.png");

    debugLog("LOADING...");
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
        if (LoadingState == EILS_WAIT)
        {
            if (--WaitCounter <= 0)
                LoadingState = EILS_SETTINGS;
        }

        else if (LoadingState == EILS_SETTINGS)
        {
            // setting determined on the fly
            core::dimension2du screenSize = Context->ScreenResolution; //Context->Renderer->getScreenSize();
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
#ifdef _IRR_ANDROID_PLATFORM_
            JNIEnv* jni = 0;
            Context->App->activity->vm->AttachCurrentThread(&jni, NULL);
            if (jni)
            {
                jclass classNativeActivity = jni->FindClass("android/app/NativeActivity");
                jclass classWindowManager = jni->FindClass("android/view/WindowManager");
                jclass classDisplay = jni->FindClass("android/view/Display");
                if (classWindowManager)
                {
                    jmethodID idNativeActivity_getWindowManager = jni->GetMethodID(classNativeActivity, "getWindowManager", "()Landroid/view/WindowManager;");
                    jmethodID idWindowManager_getDefaultDisplay = jni->GetMethodID(classWindowManager, "getDefaultDisplay", "()Landroid/view/Display;");
                    jmethodID idWindowManager_getRotation = jni->GetMethodID(classDisplay, "getRotation", "()I");
                    if (idWindowManager_getRotation)
                    {
                        jobject windowManager = jni->CallObjectMethod(Context->App->activity->clazz, idNativeActivity_getWindowManager);
                        if (windowManager)
                        {
                            jobject display = jni->CallObjectMethod(windowManager, idWindowManager_getDefaultDisplay);
                            if (display)
                            {
                                int rotation = jni->CallIntMethod(display, idWindowManager_getRotation);
                                Context->ScreenRotation = E_SCREEN_ROTATION(rotation);

                                if (rotation == 0 || rotation == 2)
                                {
                                    debugLog("default screen rotation is PORTRAIT");
                                }
                                else
                                {
                                    debugLog("default screen rotation is LANDSCAPE");
                                }
                            }
                        }
                    }
                }

                Context->App->activity->vm->DetachCurrentThread();
            }


            Context->StoragePath = core::stringc(Context->App->activity->internalDataPath) + "/";
#else
            Context->ScreenRotation = ESR_90;

            Context->StoragePath = core::stringc();
#endif

            Context->Sets->setStoragePath(Context->StoragePath);
            if (Context->ScreenRotation == ESR_0 || Context->ScreenRotation == ESR_180)
            {
                Context->Sets->AccelXBias = -1.7;
            }

            if (!Context->Sets->read())
            {
                debugLog("could not load saved settings, using default");
                Context->Sets->write();
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

                Context->Mtls->AlphaCB = new ShaderCBAlpha(Context);
                Context->Mtls->Alpha = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/alpha.vert", "shaders/alpha.frag",
                                       Context->Mtls->AlphaCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

                Context->Mtls->SolidCB = new ShaderCBSolid(Context);
                Context->Mtls->Solid = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/solid.vert", "shaders/solid.frag",
                                       Context->Mtls->SolidCB);

                Context->Mtls->FaderCB = new ShaderCBFader();
                Context->Mtls->Fader = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/fader.vert", "shaders/fader.frag",
                                       Context->Mtls->FaderCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

                Context->Mtls->Sky = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/sky.vert", "shaders/sky.frag",
                                     new ShaderCBSky());

                Context->Mtls->TracerCB = new ShaderCBTracer(Context);
                Context->Mtls->Tracer = (video::E_MATERIAL_TYPE) gpu->addHighLevelShaderMaterialFromFiles("shaders/tracer.vert", "shaders/tracer.frag",
                                        Context->Mtls->TracerCB, video::EMT_TRANSPARENT_ALPHA_CHANNEL);

                Context->Renderer->init(EET_FADER);

                Context->Mtls->Loaded = true;
            }

            LoadingState = EILS_MESHES;
            return;
        }

        else if (LoadingState == EILS_MESHES)
        {
            scene::IAnimatedMesh* line_mesh = static_cast<scene::IAnimatedMesh*>(GeometryGenerator::createLineMeshZ(1));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("line-mesh", line_mesh);
            line_mesh->drop();

            scene::IAnimatedMesh* cube_mesh = static_cast<scene::IAnimatedMesh*>(GeometryGenerator::createCubeMesh(ObjectItemCube::getCubeSize()));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh", cube_mesh);
            cube_mesh->drop();

            scene::IAnimatedMesh* cube_mesh_filled = static_cast<scene::IAnimatedMesh*>(GeometryGenerator::createCubeMesh(ObjectItemCube::getCubeSize(), true));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh-filled", cube_mesh_filled);
            cube_mesh_filled->drop();

            scene::IAnimatedMesh* pyramid_mesh = static_cast<scene::IAnimatedMesh*>(GeometryGenerator::createPyramidMesh(ObjectItemPyramid::getPyramidSize()));
            Context->Device->getSceneManager()->getMeshCache()->addMesh("pyramid-mesh", pyramid_mesh);
            pyramid_mesh->drop();

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
            debugLog("DONE");
            new ObjectStateMenu(Context);

            if (Loading)
                Loading->remove();

            delete this;
        }
    }
}
