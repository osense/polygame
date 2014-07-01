#ifndef SHADERCBDOF_H
#define SHADERCBDOF_H


#include <irrlicht.h>
#include "ShaderCBDepth.h"

using namespace irr;

class ShaderCBGrid : public video::IShaderConstantSetCallBack
{
    public:

        ShaderCBGrid(SContext* cont);

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

        void setNearColor(video::SColorf n);

        void setFarColor(video::SColorf f);

        video::SColorf getNearColor() const;

        video::SColorf getFarColor() const;

    private:
        SContext* Context;
        bool FirstUpdate;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 NearColorID, FarColorID;

        video::SColorf NearColor, FarColor;
};

#endif // SHADERCBDOF_H
