#include "ObjectPlayer.h"

ObjectPlayer::ObjectPlayer(SContext* cont) : Object(cont),
    AccSamples(AccSamplesSize)
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
        AccSamples.push_back(0);

#ifdef DEBUG_PLAYER
    DebugCamera = Context->Device->getSceneManager()->addCameraSceneNode();
    DebugCamera->setFarValue(21);
    DebugCamera->setNearValue(0.045);
    scene::ISceneNode* camVisual = Context->Device->getSceneManager()->addCubeSceneNode(0.1, Camera);
    camVisual->setScale(core::vector3df(0.5, 0.5, 1));
#endif // DEBUG_PLAYER
}

ObjectPlayer::~ObjectPlayer()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);
    Context->ObjManager->getObjectFromName("ObjectEventReceiver")->unregisterObserver(this);

    Camera->remove();

    Context->Device->deactivateAccelerometer();
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectPlayer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        /*if (Accelerating)
            Speed += Acceleration * msg.Update.fDelta;
        else
            Speed -= (Acceleration * 1.5) * msg.Update.fDelta;*/

        if (Speed > MaxSpeed)
            Speed = MaxSpeed;
        else if (Speed < MinSpeed)
            Speed = MinSpeed;

        /*if (Rising)
        {
            Energy -= msg.Update.fDelta;
            if (Energy <= 0)
                Rising = false;
            TargetRot.X = -(MaxRise - FloorAngle);
        }
        else
        {
            if (TimeTillRegen > 0)
            {
                TimeTillRegen -= msg.Update.fDelta;
            }
            else
            {
                Energy += msg.Update.fDelta * EnergyRegenSpeed;
                if (Energy > MaxEnergy)
                    Energy = MaxEnergy;
            }*/

            TargetRot.X = ((Camera->getPosition().Y - (FloorHeight + Height)) / Height) * MaxRise;
        //}

        core::vector3df rotDiff = TargetRot - Camera->getRotation();
        Camera->setRotation(Camera->getRotation() + rotDiff * (RotSpeed * msg.Update.fDelta));

        core::vector3df dir = getDirection();
        core::vector3df oldPos = Camera->getPosition();
        core::vector3df newPos = oldPos + dir * (Speed * msg.Update.fDelta);
        Speed += Acceleration * (newPos.Z - oldPos.Z);
        Camera->setPosition(newPos);
        Camera->setTarget(Camera->getPosition() + dir);

        #ifdef DEBUG_PLAYER
        DebugCamera->setPosition(Camera->getPosition() + core::vector3df(1.5, 1, 0));
        DebugCamera->setTarget(Camera->getPosition());
        #endif // DEBUG_PLAYER

        SMessage msg(this, EMT_OBJ_POS);
        core::vector3df camPos = Camera->getPosition();
        msg.Position.X = camPos.X;
        msg.Position.Y = camPos.Y;
        msg.Position.Z = camPos.Z;
        msg.Position.Speed = Speed;
        broadcastMessage(msg);
    }
    else if (msg.Type == EMT_PLAYER_FEEDBACK)
    {
        FloorHeight = msg.PlayerFeedback.Height;
        FloorAngle = msg.PlayerFeedback.GridAngle;
    }
    else if (msg.Type == EMT_INPUT)
    {
        /*if (msg.Input.Type == ETIE_PRESSED_DOWN)
            Rising = true;
        else if (msg.Input.Type == ETIE_LEFT_UP)
        {
            Rising = false;
            TimeTillRegen = EnergyRegenCooldown;
        }*/

#ifdef DEBUG_GLES
        if (msg.Input.Type == ETIE_MOVED)
        {
            const u32 screenXHalf = Context->Device->getVideoDriver()->getScreenSize().Width / 2;
            TargetRot.Y = (float(msg.Input.X) - float(screenXHalf)) / screenXHalf;
            clamp(TargetRot.Y, -1.0, 1.0);
            TargetRot.Y *= MaxAbsRotY;
        }
        else if (msg.Input.Type == ETIE_PRESSED_DOWN)
        {
            Speed += Acceleration * 10;
        }
#endif
    }
    else if (msg.Type == EMT_ACC)
    {
        f32 acc = msg.Acc.X;
        clamp(acc, -1*(AccCutoff), AccCutoff);

        AccSamples.push_back(acc);

        f32 total = 0;
        for (u32 i = 0; i < AccSamplesSize; i++)
            total += AccSamples[i];

        TargetRot.Y = (total/AccSamplesSize) * (1.0 / AccCutoff) * MaxAbsRotY;
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

        Speed = 0;

        core::vector3df camPos = Camera->getPosition();
        Camera->setTarget(camPos + core::vector3df(0, 0, 0.001));
        Camera->addAnimator(Context->Device->getSceneManager()->createFlyStraightAnimator(Camera->getPosition(),
                                                                                        Camera->getPosition() + core::vector3df(0, 0.3, -1),
                                                                                        600));
    }
    else if (msg.Type == EMT_SERIALIZE)
    {
        Json::Value playerRoot;

        storeVector3df(Camera->getPosition(), playerRoot, "pos");
        storeVector3df(Camera->getRotation(), playerRoot, "rot");
        playerRoot["speed"] = Speed;
        playerRoot["floor_height"] = FloorHeight;
        playerRoot["floor_angle"] = FloorAngle;
        storeVector3df(TargetRot, playerRoot, "target_rot");

        (*msg.SData.Root)[Name.c_str()] = playerRoot;
    }
    else if (msg.Type == EMT_DESERIALIZE)
    {
        Json::Value playerRoot = (*msg.SData.Root)[Name.c_str()];

        Camera->setPosition(parseVector3df(playerRoot, "pos"));
        Camera->setRotation(parseVector3df(playerRoot, "rot"));
        Speed = playerRoot["speed"].asDouble();
        FloorHeight = playerRoot["floor_height"].asDouble();
        FloorAngle = playerRoot["floor_angle"].asDouble();
        TargetRot = parseVector3df(playerRoot, "target_rot");
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

core::vector3df ObjectPlayer::getTargetDirection() const
{
    core::vector3df rot = TargetRot;

    return core::vector3df(sin(core::degToRad(rot.Y)), -sin(core::degToRad(rot.X)), cos(core::degToRad(rot.Y))).normalize();
}

f32 ObjectPlayer::getHeight() const
{
    return Camera->getPosition().Y - FloorHeight;
}

f32 ObjectPlayer::getDT() const
{
    return Camera->getPosition().Z;
}

scene::ICameraSceneNode* ObjectPlayer::getCamera() const
{
    return Camera;
}
