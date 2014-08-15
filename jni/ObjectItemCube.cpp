#include "ObjectItemCube.h"

ObjectItemCube::ObjectItemCube(SContext* cont, core::vector3df pos) : ObjectItem(cont)
{
    Name = "ObjectItemCube";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectPlayer")->registerObserver(this);

    scene::ISceneManager* smgr = Context->Device->getSceneManager();
    Node = smgr->addMeshSceneNode(Context->Device->getSceneManager()->getMesh("cube-mesh"));
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->ItemCube);

    Node->setPosition(pos);
}

ObjectItemCube::~ObjectItemCube()
{
    Context->ObjManager->getObjectFromName("ObjectUpdater")->unregisterObserver(this);

    if (State == EIS_ITEM)
    {
        Node->remove();

        Object* pl = Context->ObjManager->getObjectFromName("ObjectPlayer");
        if (pl)
            pl->unregisterObserver(this);
    }

    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_DIED));
}

void ObjectItemCube::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (State == EIS_ITEM)
        {
            Node->setRotation(Node->getRotation() + core::vector3df(RotSpeed) * msg.Update.Delta);
        }
        else if (State == EIS_EFFECT_FADEIN)
        {
            Context->Mtls->GridBackCB->setAlpha(Context->Mtls->GridBackCB->getAlpha() - msg.Update.fDelta * (1/TimeFadein));

            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= TimeActive - TimeFadein)
                State = EIS_EFFECT;
        }
        else if (State == EIS_EFFECT)
        {
            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= TimeFadein)
                State = EIS_EFFECT_FADEOUT;
        }
        else if (State == EIS_EFFECT_FADEOUT)
        {
            Context->Mtls->GridBackCB->setAlpha(Context->Mtls->GridBackCB->getAlpha() + msg.Update.fDelta * (1/TimeFadein));

            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= 0)
            {
                static_cast<ObjectGrid*>(Context->ObjManager->getObjectFromName("ObjectGrid"))->setCollision(true);
                delete this;
            }
        }

    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        if (Node->getPosition().Z + DestroyDist <= msg.Position.Z)
        {
            delete this;
            return;
        }
        if ((Node->getPosition() - core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z)).getLength() <= CubeSize*1.5)
        {
            static_cast<ObjectGrid*>(Context->ObjManager->getObjectFromName("ObjectGrid"))->setCollision(false);
            EffectCounter = TimeActive;
            State = EIS_EFFECT_FADEIN;

            Node->remove();
            Context->ObjManager->getObjectFromName("ObjectPlayer")->unregisterObserver(this);
        }
    }
}

scene::IMesh* ObjectItemCube::getCubeMesh()
{
    scene::IAnimatedMesh* mesh = Context->Device->getSceneManager()->getMesh("cube-mesh");
    if (!mesh)
    {
        GeometryGenerator geomGen;
        mesh = static_cast<scene::IAnimatedMesh*>(geomGen.createCubeMesh(CubeSize));
        Context->Device->getSceneManager()->getMeshCache()->addMesh("cube-mesh", mesh);
        mesh->drop();
    }

    return mesh;
}
