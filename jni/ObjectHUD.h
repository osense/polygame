#ifndef OBJECTHUD_H
#define OBJECTHUD_H

#include "Object.h"
#include "ObjectManager.h"
#include "ObjectPlayer.h"
#include "functions.h"

enum E_HUD_ELEMENT_VIS
{
    EHEV_ON,
    EHEV_OFF,
    EHEV_FADEOUT,
    EHEV_FADEIN,
    EHEV_WAIT_FADEOUT
};


class ObjectHUD : public Object
{
    public:
        ObjectHUD(SContext* cont);
        virtual ~ObjectHUD();

        virtual void onMessage(SMessage msg);

    private:
        video::SColor HUDColor, FadeHUDColor;
        f32 DTFadeCounter, EnergyFadeCounter;

        static constexpr f32 DTFadeTime = 1.5;
        static constexpr u32 DisplayDTEvery = 50;
        static constexpr u32 DisplayDTFrom = 3 + DTFadeTime;
        static constexpr u32 DisplayDTTo = 5 + DTFadeTime;
        // pixels per energy
        static constexpr u32 PPE = 50;
        static constexpr f32 EnergyX = 7;
        static constexpr f32 EnergyY = 455;
        static constexpr f32 EnergyFadeTime = 0.5;

        gui::IGUIStaticText* DistTravelledText;
        E_HUD_ELEMENT_VIS DistVisibility;
        f32 DT;

        video::ITexture* VLineSegment;
        E_HUD_ELEMENT_VIS EnergyVisibility;
        core::array<core::position2d< s32 > > EnergyPositions;
        core::array<core::rect< s32 > > EnergyRects;
};

#endif // OBJECTHUD_H
