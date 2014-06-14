#include "ObjectPlayer.h"

ObjectPlayer::ObjectPlayer(SContext* cont) : Object(cont)
{
    Name = "ObjectPlayer";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);

    scene::ISceneManager* smgr = Context->Device->getSceneManager();

    #ifdef DEBUG_GLES
    Camera = smgr->addCameraSceneNodeFPS(0, 100, 0.015);

    #else
    Context->Device->activateGyroscope();
    Camera = smgr->addCameraSceneNode();

    #endif // DEBUG_GLES

    Camera->setPosition(core::vector3df(0, 2, 0));
}

ObjectPlayer::~ObjectPlayer()
{
    Object* updater = Context->ObjManager->getObjectFromName("ObjectUpdater");
    if (updater)
        updater->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectPlayer::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        Camera->setPosition(Camera->getPosition() + core::vector3df(0, 0, 0.02));

        SMessage msg(this, EMT_OBJ_POS);
        core::vector3df camPos = Camera->getPosition();
        msg.Position.X = camPos.X;
        msg.Position.Y = camPos.Y;
        msg.Position.Z = camPos.Z;
        broadcastMessage(msg);
    }
}
