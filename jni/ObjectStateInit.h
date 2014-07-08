#ifndef OBJECTSTATEINIT_H
#define OBJECTSTATEINIT_H

#include "Object.h"
#include "ObjectManager.h"
#include "ObjectStateMenu.h"

#include "EffectRenderer.h"
#include "ShaderCBDepth.h"
#include "ShaderCBGrid.h"
#include "ShaderCBGridBack.h"
#include "ShaderCBSky.h"


enum E_INIT_LOADING_STATE
{
    EILS_RENDERER = 0,
    EILS_SHADERS,
    EILS_TEXTURES,
    EILS_FONTS,
    EILS_DONE
};

using namespace irr;

class ObjectStateInit : public Object
{
    public:
        ObjectStateInit(SContext* cont);

        ~ObjectStateInit();

        virtual void onMessage(SMessage msg);

    private:
        E_INIT_LOADING_STATE LoadingState;

        u32 TexturesLoaded;
        core::array<core::stringc> TextureNames;

        gui::IGUIImage* Image;
};

#endif // OBJECTSTATEINIT_H
