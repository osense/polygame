#include <irrlicht.h>

#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#include "ObjectWakeLock.h"
#endif

#include "SContext.h"

#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "ObjectUpdaterThreaded.h"
#include "ObjectEventReceiver.h"
#include "EffectRenderer.h"

#ifdef DEBUG_INFO
#include "ObjectDebugInfo.h"
#endif // DEBUG_INFO

#include "ObjectStateInit.h"


using namespace irr;

#ifdef _IRR_ANDROID_PLATFORM_
void android_main(android_app* app)
#else
int main(int argc, char *argv[])
#endif
{
    IrrlichtDevice* dev = 0;

#ifdef _IRR_ANDROID_PLATFORM_
    //Make sure glue isn't stripped.
    app_dummy();

    SIrrlichtCreationParameters param;
    param.DriverType = video::EDT_OGLES2;
    param.WindowSize = core::dimension2d<u32>(0, 0);
    param.PrivateData = app;
    param.OGLES2ShaderPath = "shaders/Irrlicht/";
    param.ZBufferBits = 16;
    param.AntiAlias = 0;

    dev = createDeviceEx(param);
#elif DEBUG_GLES
    SIrrlichtCreationParameters param;
    param.DriverType = video::EDT_OGLES2;
    param.WindowSize = core::dimension2d<u32>(DESKTOP_WND_X, DESKTOP_WND_Y);
    param.OGLES2ShaderPath = "assets/shaders/Irrlicht/";
    param.Bits = 32;
    dev = createDeviceEx(param);

    dev->setWindowCaption(L"Polygame Desktop GLES Debug Build    " __DATE__ " " __TIME__);
    dev->getFileSystem()->changeWorkingDirectoryTo(dev->getFileSystem()->getAbsolutePath("assets/"));
#endif

    SContext* cont = new SContext();
#ifdef _IRR_ANDROID_PLATFORM_
    cont->App = app;
#endif
    cont->Device = dev;

    cont->ObjManager = new ObjectManager(cont);

#ifdef DEBUG_INFO
    new ObjectDebugInfo(cont);
    dev->getLogger()->setLogLevel(ELL_DEBUG);
#endif // DEBUG_FPS

    cont->Mtls = new SMaterials;
    cont->Mtls->Loaded = false;
    
    cont->Sets = new Settings(cont);
    cont->TimeScale = 1;

    new ObjectEventReceiver(cont);

    ObjectUpdater* updater = new ObjectUpdater(cont);
    new ObjectUpdaterThreaded(cont);

    cont->Renderer = new EffectRenderer(cont);

#ifdef _IRR_ANDROID_PLATFORM_
    new ObjectWakeLock(cont);
#endif // _IRR_ANDROID_PLATFORM_

    dev->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);


    new ObjectStateInit(cont);


    ITimer* timer = dev->getTimer();
    u32 timeLast = timer->getTime();

    while(dev->run())
    {
        u32 timeNow = timer->getTime();
        u32 timeDelta = timeNow - timeLast;
        timeLast = timeNow;

        dev->getVideoDriver()->beginScene();
        cont->Renderer->drawAll(timeDelta);

        updater->broadcastUpdate(timeDelta * cont->TimeScale);

        dev->getVideoDriver()->endScene();
    }

    cont->ObjManager->clear();
    cont->ObjManager->clear(true);
    delete cont->ObjManager;
    delete cont->Mtls;
    delete cont;
#ifndef _IRR_ANDROID_PLATFORM_
    return 0;
#endif
}

