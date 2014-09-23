#ifndef OBJECTSTATEOPTIONS_H
#define OBJECTSTATEOPTIONS_H


#include "Object.h"
#include "ObjectManager.h"
#include "ObjectStateInit.h"
#include "ObjectStateOptionsCalibration.h"
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
    EOI_SEED_0 = 0,
    EOI_SEED_1 = 1,
    EOI_SEED_2 = 2,
    EOI_SEED_3 = 3,
    EOI_SEED_4 = 4,
    EOI_SEED_5 = 5,
    EOI_SEED_6 = 6,
    EOI_SEED_7 = 7,
    EOI_SEED_8 = 8,
    EOI_SEED_9 = 9,
    EOI_SEED_C,
    EOI_SEED_BOX,

    EOI_GFX,
    EOI_CONTROLS,
    EOI_CONTROLS_TILT,
    EOI_CONTROLS_TOUCH,
    EOI_CONTROLS_CALIBRATE,
    EOI_CONTROLS_CALIBRATE_BAR,
    EOI_CONTROLS_CALIBRATE_BAR_DOT,
    EOI_SEED,
    EOI_BACK,
    EOI_GLOW_OFF,
    EOI_GLOW_LOW,
    EOI_GLOW_MEDIUM,
    EOI_GLOW_HIGH
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

    void controls_onTiltSelected();
    void controls_onTouchSelected();
    void seed_onDigitPressed(u32 digit);

    void setVertLineVisible(bool vis);
    void serialize();
    void deserialize();

    E_OPTIONS_STATE State;
    gui::IGUIWindow* MainWindow, *PaneWindow;
    scene::ICameraSceneNode* Camera;

    video::ITexture* HLineSegment, *VLineSegment;
    core::array<core::position2d< s32 > > LinePositions;
    core::array<core::rect< s32 > > LineRects;
    bool VertLineVisible = false;
    core::array<core::position2d< s32 > > VertLinePositions;
    core::array<core::rect< s32 > > VertLineRects;


};

#endif // OBJECTSTATEOPTIONS_H
