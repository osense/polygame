#include "ObjectPlayer.h"

ObjectPlayer::ObjectPlayer(SContext* cont) : Object(cont),
    Speed(MinSpeed),
    TargetRotY(0),
    MaxAbsRotY(DefaultMaxAbsRotY),
    Accelerating(false),
    AccLastSampleIdx(0)
{
    Name = "ObjectPlayer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);

#ifdef _IRR_ANDROID_PLATFORM_
    Context->Device->activateAccelerometer(0.02);
#endif

    Camera = Context->Device->getSceneManager()->addCameraSceneNode();
    Camera->setFarValue(20);
    Camera->setNearValue(0.05);
    Camera->setPosition(core::vector3df(0, 0.4, 0));

    for (u32 i = 0 ; i < AccSamplesSize; i++)
        AccSamples[i] = 0;

#ifdef DEBUG_PLAYER
    DebugCamera = Context->Device->getSceneManager()->addCameraSceneNode();
    DebugCamera->setFarValue(21);
    DebugCamera->setNearValue(0.05);
    scene::ISceneNode* camVisual = Context->Device->getSceneManager()->addCubeSceneNode(0.1, Camera);
    camVisual->setScale(core::vector3df(0.5, 0.5, 1));
#endif // DEBUG_PLAYER
}

ObjectPlayer::~ObjectPlayer()
{
    Object* updater = Context->ObjManager->getObjectFromName("ObjectUpdater");
    if (updater)
        updater->unregisterObserver(this);

    Context->Device->deactivateAccelerometer();
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectPlayer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (Accelerating)
            Speed += Acceleration * msg.Update.fDelta;
        else
            Speed -= (Acceleration * 1.5) * msg.Update.fDelta;

        if (Speed > MaxSpeed)
            Speed = MaxSpeed;
        else if (Speed < MinSpeed)
            Speed = MinSpeed;

        Camera->setRotation((Camera->getRotation() + core::vector3df(0, TargetRotY, 0)) / 2.0);

        core::vector3df dir = getDirection();
        Camera->setPosition(Camera->getPosition() + dir * Speed);
        Camera->setTarget(Camera->getPosition() + dir);

        #ifdef DEBUG_PLAYER
        DebugCamera->setPosition(Camera->getPosition() + core::vector3df(0, 0.5, -1));
        DebugCamera->setTarget(Camera->getPosition());
        #endif // DEBUG_PLAYER

        //Quad->setMaterialTexture(0, Context->Renderer->getCrashEffectTexture());
        //Quad->setMaterialType(Context->Mtls->ColorBlend);
        //Quad->render();
        /*video::IVideoDriver* video = Context->Device->getVideoDriver();
        video::ITexture* crashEffect =
        video->draw2DImage(crashEffect, core::position2d<s32>(0, 0), core::rect<s32>(core::dimension2d<s32>(0, 0), crashEffect->getSize()), 0, video::SColor(255, 255, 255, 0), true);*/

        SMessage msg(this, EMT_OBJ_POS);
        core::vector3df camPos = Camera->getPosition();
        msg.Position.X = camPos.X;
        msg.Position.Y = camPos.Y;
        msg.Position.Z = camPos.Z;
        broadcastMessage(msg);
    }
    else if (msg.Type == EMT_INPUT)
    {
        if (msg.Input.Type == ETIE_PRESSED_DOWN)
            Accelerating = true;
        else if (msg.Input.Type == ETIE_LEFT_UP)
            Accelerating = false;

#ifdef DEBUG_GLES
        else if (msg.Input.Type == ETIE_MOVED)
        {
            const u32 screenXHalf = Context->Device->getVideoDriver()->getScreenSize().Width / 2;
            TargetRotY = (float(msg.Input.X) - float(screenXHalf)) / screenXHalf;
            clamp(TargetRotY, -1.0, 1.0);
            TargetRotY *= MaxAbsRotY;
        }
#endif
    }
    else if (msg.Type == EMT_ACC)
    {
        f32 accY = msg.Acc.Y;
        clamp(accY, -1*(AccCutoff), AccCutoff);

        AccLastSampleIdx++;
        if (AccLastSampleIdx >= AccSamplesSize)
            AccLastSampleIdx = 0;

        AccSamples[AccLastSampleIdx] = accY;

        f32 total = 0;
        for (u32 i = 0; i < AccSamplesSize; i++)
            total += AccSamples[i];

        TargetRotY = (total/AccSamplesSize) * (1.0 / AccCutoff) * MaxAbsRotY;
    }
}


f32 ObjectPlayer::getSpeed() const
{
    return Speed;
}

core::vector3df ObjectPlayer::getDirection() const
{
    core::vector3df rot = Camera->getRotation();

    return core::vector3df(sin(core::degToRad(rot.Y)), -sin(core::degToRad(rot.X)), cos(core::degToRad(rot.Y))).normalize();
}
