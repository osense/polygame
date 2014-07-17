#include "ObjectSky.h"

ObjectSky::ObjectSky(SContext* cont) : Object(cont)
{
    Name = "ObjectSky";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    scene::ISceneManager* smgr = Context->Device->getSceneManager();

    SkyNode = smgr->addSkyDomeSceneNode(0, 16, 18, 0.9, 2.0, 19);
    SkyNode->setMaterialType(Context->Mtls->Sky);
    //SkyNode->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
    SkyNode->setMaterialTexture(0, Context->Device->getVideoDriver()->getTexture("noise.png"));

    SkyNode->setRotation(core::vector3df(0, 0, 90));
    SkyNode->addAnimator(smgr->createRotationAnimator(core::vector3df(0, -0.005, 0)));
}

ObjectSky::~ObjectSky()
{
    SkyNode->remove();

    Object* player = Context->ObjManager->getObjectFromName("ObjectPlayer");
    if (player)
        player->unregisterObserver(this);

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectSky::onMessage(SMessage msg)
{
    if (msg.Type == EMT_OBJ_POS)
    {
        SkyNode->setPosition(core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z));
    }
    else if (msg.Type == EMT_OBJ_SPAWNED)
    {
        if (msg.Dispatcher->getName() == "ObjectPlayer")
            msg.Dispatcher->registerObserver(this);
    }
}
