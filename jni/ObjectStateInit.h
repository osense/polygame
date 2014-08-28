#ifndef OBJECTSTATEINIT_H
#define OBJECTSTATEINIT_H

#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#endif // _IRR_ANDROID_PLATFORM_

#include "Object.h"
#include "ObjectManager.h"
#include "ObjectStateMenu.h"

#include "EffectRenderer.h"

#include "GeometryGenerator.h"
#include "ObjectItemCube.h"
#include "ObjectItemPyramid.h"


enum E_INIT_LOADING_STATE
{
    EILS_WAIT = 0,
    EILS_SETTINGS,
    EILS_RENDERER,
    EILS_SHADERS,
    EILS_MESHES,
    EILS_TEXTURES,
    EILS_FONTS,
    EILS_DONE
};

using namespace irr;

class ObjectStateInit : public Object
{
    public:
        ObjectStateInit(SContext* cont, bool showLoading = true);

        ~ObjectStateInit();

        virtual void onMessage(SMessage msg);

    private:
        E_INIT_LOADING_STATE LoadingState;
        u32 WaitCounter;

        u32 TexturesLoaded;
        core::array<core::stringc> TextureNames;

        gui::IGUIStaticText* Loading;
};

#endif // OBJECTSTATEINIT_H
