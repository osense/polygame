#ifndef OBJECTHUD_H
#define OBJECTHUD_H

#include "Object.h"
#include "ObjectManager.h"
#include "ObjectPlayer.h"
#include "functions.h"


class ObjectHUD : public Object
{
    public:
        ObjectHUD(SContext* cont);
        virtual ~ObjectHUD();

        virtual void onMessage(SMessage msg);

    private:
        // pixels per energy
        static constexpr u32 PPE = 50;
        static constexpr f32 EnergyX = 7;
        static constexpr f32 EnergyY = 455;

        gui::IGUIStaticText* DistTravelledText;
        f32 DT;

        video::ITexture* VLineSegment;
        core::array<core::position2d< s32 > > EnergyPositions;
        core::array<core::rect< s32 > > EnergyRects;
};

#endif // OBJECTHUD_H
