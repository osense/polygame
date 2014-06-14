#include <irrlicht.h>

#ifdef _IRR_ANDROID_PLATFORM_
	#include <android_native_app_glue.h>
#endif

#include "SContext.h"
#include "ObjectUpdater.h"
#include "ObjectStateGame.h"
#include "ObjectDebugInfo.h"

#include "ShaderCBSimple.h"
#include "ObjectGrid.h"

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
#else
    dev = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(DESKTOP_WND_X, DESKTOP_WND_Y), 32, false);
    dev->setWindowCaption(L"IrrGame Desktop Debug Build    " __DATE__ " " __TIME__);
    dev->getFileSystem()->changeWorkingDirectoryTo(dev->getFileSystem()->getAbsolutePath("assets/"));
#endif

    SContext* cont = new SContext();
    cont->Device = dev;

    cont->ObjManager = new ObjectManager(cont);
    ObjectUpdater* updater = new ObjectUpdater(cont);
    new ObjectDebugInfo(cont);
    //SContext->Device->getVideoDriver()->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);


    scene::ICameraSceneNode* cam = dev->getSceneManager()->addCameraSceneNodeFPS(0, 100, 0.015);
    cam->setPosition(core::vector3df(0, 2.5, 0));
    cam->setTarget(core::vector3df(-5, 0, 10));

    /*scene::ISceneManager* smgr = dev->getSceneManager();
    scene::IMeshSceneNode* plane = smgr->addMeshSceneNode(smgr->getMesh("plane.obj"));
    video::E_MATERIAL_TYPE mat = (video::E_MATERIAL_TYPE)dev->getVideoDriver()->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles("test.vert", "test.frag", new ShaderCBSimple(cont));
    plane->setMaterialType(mat);
    plane->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    plane->setMaterialFlag(video::EMF_WIREFRAME, true);*/

    //plane->addAnimator(smgr->createRotationAnimator(core::vector3df(0, 0.05, 0)));

    new ObjectGrid(cont);


    ITimer* timer = dev->getTimer();
    u32 timeLast = timer->getTime();

    video::SMaterial mat;
    mat.Thickness = 5.0;

    while(dev->run())
    {
        u32 timeNow = timer->getTime();
        u32 timeDelta = timeNow - timeLast;
        timeLast = timeNow;

        dev->getVideoDriver()->beginScene();
        dev->getSceneManager()->drawAll();
        dev->getGUIEnvironment()->drawAll();

        updater->broadcastUpdate(timeDelta);

        dev->getVideoDriver()->endScene();
    }


#ifndef _IRR_ANDROID_PLATFORM_
	return 0;
#endif
}

