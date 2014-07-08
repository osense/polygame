#include <irrlicht.h>

#ifdef _IRR_ANDROID_PLATFORM_
	#include <android_native_app_glue.h>
#endif

#include "SContext.h"

#include "ObjectManager.h"
#include "ObjectUpdater.h"
#include "ObjectEventReceiver.h"
#include "EffectRenderer.h"

#ifdef DEBUG_FPS
#include "ObjectDebugInfo.h"
#endif // DEBUG_FPS

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
    param.OGLES2ShaderPath = "shaders/";
    param.Bits = 24;
    param.ZBufferBits = 16;
    param.AntiAlias = 0;

    dev = createDeviceEx(param);
#elif DEBUG_GLES
    SIrrlichtCreationParameters param;
    param.DriverType = video::EDT_OGLES2;
    param.WindowSize = core::dimension2d<u32>(DESKTOP_WND_X, DESKTOP_WND_Y);
    param.OGLES2ShaderPath = "assets/shaders/";
    param.Bits = 32;
    dev = createDeviceEx(param);

    dev->setWindowCaption(L"Polygame Desktop GLES Debug Build    " __DATE__ " " __TIME__);
    dev->getFileSystem()->changeWorkingDirectoryTo(dev->getFileSystem()->getAbsolutePath("assets/"));
#endif

    SContext* cont = new SContext();
    cont->Device = dev;
    f32 gScaleX = dev->getVideoDriver()->getScreenSize().Width / float(DESKTOP_WND_X);
    f32 gScaleY = dev->getVideoDriver()->getScreenSize().Height / float(DESKTOP_WND_Y);
    cont->GUIScale = gScaleX > gScaleY ? gScaleX : gScaleY;

    cont->ObjManager = new ObjectManager(cont);

#ifdef DEBUG_FPS
    new ObjectDebugInfo(cont);
#endif // DEBUG_FPS

    cont->Mtls = new SMaterials;
    ObjectUpdater* updater = new ObjectUpdater(cont);
    new ObjectEventReceiver(cont);
    cont->Renderer = new EffectRenderer(cont);

    new ObjectStateInit(cont);


    ITimer* timer = dev->getTimer();
    u32 timeLast = timer->getTime();

    while(dev->run())
    {
        u32 timeNow = timer->getTime();
        u32 timeDelta = timeNow - timeLast;
        timeLast = timeNow;

        dev->getVideoDriver()->beginScene();
        cont->Renderer->drawAll();
        dev->getGUIEnvironment()->drawAll();

        updater->broadcastUpdate(timeDelta);

        dev->getVideoDriver()->endScene();
    }


#ifndef _IRR_ANDROID_PLATFORM_
	return 0;
#endif
}

