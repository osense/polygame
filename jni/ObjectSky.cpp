#include "ObjectSky.h"

ObjectSky::ObjectSky(SContext* cont) : Object(cont),
    CurrentRotSpeed(RotSpeed)
{
    Name = "ObjectSky";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    SkyNode = Context->Device->getSceneManager()->addSkyDomeSceneNode(0, 16, 18, 0.9, 2.0, 19);
    SkyNode->setMaterialType(Context->Mtls->Sky);
    SkyNode->setMaterialTexture(0, Context->Device->getVideoDriver()->getTexture("textures/noise.png"));

    SkyNode->setRotation(core::vector3df(0, 0, 90));
}

ObjectSky::~ObjectSky()
{
    SkyNode->remove();

    Object* player = Context->ObjManager->getObjectFromName("ObjectPlayer");
    if (player)
        player->unregisterObserver(this);

    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectSky::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        SkyNode->setPosition(core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z));
        CurrentRotSpeed = RotSpeed * msg.Position.Speed;
    }
    else if (msg.Type == EMT_UPDATE)
    {
        SkyNode->setRotation(SkyNode->getRotation() + core::vector3df(0, CurrentRotSpeed * msg.Update.Delta, 0));
    }
    else if (msg.Type == EMT_PLAYER_CRASHED)
    {
        CurrentRotSpeed = 0;
    }
    else if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            msg.Dispatcher->registerObserver(this);
    }
}
