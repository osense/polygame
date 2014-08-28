#ifndef SHADERCBDOF_H
#define SHADERCBDOF_H

#include <irrlicht.h>
#include "SContext.h"

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

        void setTransform(f32 t);

        f32 getTransform() const;

    private:
        SContext* Context;
        bool FirstUpdate;

        video::SColorf NearColor, FarColor;
        f32 Transform;

        s32 CamFarID;
        s32 WorldViewProjMatID, WorldViewMatID;
        s32 NearColorID, FarColorID;
        s32 TransformID;
};

#endif // SHADERCBDOF_H
