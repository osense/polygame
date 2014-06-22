#include "ObjectPlayer.h"

ObjectPlayer::ObjectPlayer(SContext* cont) : Object(cont),
    Speed(MinSpeed),
    Accelerating(false)
{
    Name = "ObjectPlayer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->registerObserver(this);

    scene::ISceneManager* smgr = Context->Device->getSceneManager();

    #ifdef DEBUG_GLES
    Camera = smgr->addCameraSceneNodeFPS(0, 100, 0.015);

    #else
    Context->Device->activateAccelerometer();
    Camera = smgr->addCameraSceneNode();

    #endif

    Camera->setFarValue(20);
    Camera->setPosition(core::vector3df(0, 2, 0));
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

        core::vector3df dir = getDirection();
        Camera->setPosition(Camera->getPosition() + dir * Speed);
        #ifndef DEBUG_GLES
        Camera->setTarget(Camera->getPosition() + dir);
        #endif // DEBUG_GLES

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
        Camera->setRotation(Camera->getRotation() + core::vector3df(msg.Acc.Z * 0.1, msg.Acc.Y * 0.1, 0));
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
