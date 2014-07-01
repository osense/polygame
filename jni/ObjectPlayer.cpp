#include "ObjectPlayer.h"

ObjectPlayer::ObjectPlayer(SContext* cont) : Object(cont),
    Speed(MinSpeed),
    Accelerating(false)
{
    Name = "ObjectPlayer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);

    #ifdef _IRR_ANDROID_PLATFORM_
    Context->Device->activateAccelerometer();
    #endif

    Camera = Context->Device->getSceneManager()->addCameraSceneNode();
    Camera->setFarValue(25);
    Camera->setNearValue(0.05);
    Camera->setPosition(core::vector3df(0, 0.4, 0));

    //Quad = Context->Renderer->PP->getQuadNode();
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

        Camera->setRotation(Camera->getRotation() + core::vector3df(0, -(Camera->getRotation().Y*msg.Update.fDelta), 0));

        debugLog(core::stringc(Camera->getRotation().Y).c_str());

        core::vector3df dir = getDirection();
        Camera->setPosition(Camera->getPosition() + dir * Speed);
        #ifndef DEBUG_GLES
        Camera->setTarget(Camera->getPosition() + dir);
        #endif // DEBUG_GLES

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
    }
    else if (msg.Type == EMT_ACC)
    {
        //Camera->setRotation(Camera->getRotation() + core::vector3df(msg.Acc.Z * 0.1, msg.Acc.Y * 0.1, 0));
        Camera->setRotation(Camera->getRotation() + core::vector3df(0, msg.Acc.Y * 0.2, 0));
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
