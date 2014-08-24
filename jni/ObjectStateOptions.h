#ifndef OBJECTSTATEOPTIONS_H
#define OBJECTSTATEOPTIONS_H


#include "Object.h"
#include "ObjectManager.h"
#include "ObjectStateInit.h"
#include "EffectRenderer.h"
#include "functions.h"
#include <irrlicht.h>

using namespace irr;

enum E_OPTIONS_STATE
{
    EOS_GFX,
    EOS_CONTROLS,
    EOS_SEED
};

enum E_OPTIONS_ID
{
    EOI_GFX,
    EOI_CONTROLS,

    EOI_SEED,
    EOI_SEED_BOX,
    EOI_SEED_1,
    EOI_SEED_2,
    EOI_SEED_3,
    EOI_SEED_4,
    EOI_SEED_5,
    EOI_SEED_6,
    EOI_SEED_7,
    EOI_SEED_8,
    EOI_SEED_9,
    EOI_SEED_0,
    EOI_SEED_C,

    EOI_BACK,
    EOI_GLOW_OFF,
    EOI_GLOW_LOW,
    EOI_GLOW_MEDIUM,
    EOI_GLOW_HIGH,
    EOI_FXAA_OFF,
    EOI_FXAA_ON
};

class ObjectStateOptions : public Object
{
    public:
        ObjectStateOptions(SContext* cont);

        virtual ~ObjectStateOptions();

        virtual void onMessage(SMessage msg);

    private:
        void create_gui();
        void create_gfx();
        void create_controls();
        void create_seed();

        void serialize();
        void deserialize();

        E_OPTIONS_STATE State;
        gui::IGUIWindow* MainWindow, *PaneWindow;
        scene::ICameraSceneNode* Camera;

        video::ITexture* HLineSegment, *VLineSegment;
        core::array<core::position2d< s32 > > LinePositions;
        core::array<core::rect< s32 > > LineRects;
        core::array<core::position2d< s32 > > GfxLinePositions;
        core::array<core::rect< s32 > > GfxLineRects;


};

#endif // OBJECTSTATEOPTIONS_H
