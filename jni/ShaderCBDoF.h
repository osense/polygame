#ifndef SHADERCBDOF_H
#define SHADERCBDOF_H


#include <irrlicht.h>

using namespace irr;

class ShaderCBDoF : public video::IShaderConstantSetCallBack
{
    public:

        void OnSetConstants(video::IMaterialRendererServices* services, s32 userData);

    private:
        bool FirstUpdate;

        s32 SceneID, DepthID;

};

#endif // SHADERCBDOF_H
