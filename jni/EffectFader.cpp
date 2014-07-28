#include "EffectFader.h"

EffectFader::EffectFader(SContext* cont) : Context(cont),
    IncludeGUI(false)
{
    MeshBuffer = new scene::SMeshBuffer();

    MeshBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-1.0, 1.0, 0.0), core::vector3df(0.0, 0.0, 1.0), video::SColor(255,255,255,255), core::vector2df(0.0, 1.0)));
    MeshBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(1.0, 1.0, 0.0), core::vector3df(0.0, 0.0, 1.0), video::SColor(255,255,255,255), core::vector2df(1.0, 1.0)));
    MeshBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(1.0, -1.0, 0.0), core::vector3df(0.0, 0.0, 1.0), video::SColor(255,255,255,255), core::vector2df(1.0, 0.0)));
    MeshBuffer->Vertices.push_back(video::S3DVertex(core::vector3df(-1.0, -1.0, 0.0), core::vector3df(0.0, 0.0, 1.0), video::SColor(255,255,255,255), core::vector2df(0.0, 0.0)));
    MeshBuffer->Indices.push_back(0); MeshBuffer->Indices.push_back(1); MeshBuffer->Indices.push_back(2);
    MeshBuffer->Indices.push_back(3); MeshBuffer->Indices.push_back(0); MeshBuffer->Indices.push_back(2);
    MeshBuffer->recalculateBoundingBox();

    MeshBuffer->getMaterial().MaterialType = Context->Mtls->Fader;
    MeshBuffer->getMaterial().setFlag(video::EMF_ZBUFFER, false);
    MeshBuffer->getMaterial().setFlag(video::EMF_ZWRITE_ENABLE, false);

    State = EFS_NONE;
}

EffectFader::~EffectFader()
{
    delete Node->getMesh();
    Node->remove();
}

void EffectFader::startFadeOut(f32 strength, f32 fadeTime, f32 strengthStart)
{
    State = EFS_FADEOUT;
    TargetStrength = strength;
    FadeTime = fadeTime;
    StartStrength = strengthStart;
    Context->Mtls->FaderCB->setAlpha(strengthStart);
}

void EffectFader::startFadeInContinuous()
{
    State = EFS_FADEIN;
    TargetStrength = StartStrength;
}

void EffectFader::startFadeIn(f32 strengthStart, f32 fadeTime)
{
    State = EFS_FADEIN;
    Strength = strengthStart;
    TargetStrength = 0;
    FadeTime = fadeTime;
    Context->Mtls->FaderCB->setAlpha(strengthStart);
}

void EffectFader::draw(f32 delta)
{
    if (isActive())
    {
        ShaderCBFader* fader = Context->Mtls->FaderCB;

        if (State == EFS_FADEOUT)
        {
            fader->setAlpha(fader->getAlpha() + delta * (1/FadeTime));

            if (fader->getAlpha() >= TargetStrength)
                State = EFS_FADED_OUT;
        }
        else if (State == EFS_FADEIN)
        {
            fader->setAlpha(fader->getAlpha() - delta * (1/FadeTime));

            if (fader->getAlpha() <= TargetStrength)
                State = EFS_NONE;
        }

        Context->Device->getVideoDriver()->drawMeshBuffer(MeshBuffer);
    }
}

bool EffectFader::isActive() const
{
    return State != EFS_NONE;
}

void EffectFader::setIncludeGUI(bool incl)
{
    IncludeGUI = incl;
}

bool EffectFader::getIncludeGUI() const
{
    return IncludeGUI;
}
