#ifndef OBJECTSTATEOPTIONS_H
#define OBJECTSTATEOPTIONS_H


#include "Object.h"
#include "ObjectManager.h"
#include "ObjectStateInit.h"
#include "EffectRenderer.h"
#include "functions.h"
#include <irrlicht.h>

using namespace irr;

enum E_OPTIONS_ID
{
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
        void serialize();
        void deserialize();

        gui::IGUIWindow* Window;
        scene::ICameraSceneNode* Camera;

        static const u32 LineSpacing = 75;
        video::ITexture* LineSegment;
        core::array<core::position2d< s32 > > LinePositions;
        core::array<core::rect< s32 > > LineRects;


};

#endif // OBJECTSTATEOPTIONS_H
