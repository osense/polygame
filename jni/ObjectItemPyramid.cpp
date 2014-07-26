#include "ObjectItemPyramid.h"

ObjectItemPyramid::ObjectItemPyramid(SContext* cont, core::vector3df pos) : ObjectItem(cont)
{
    Name = "ObjectItemCube";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectPlayer")->registerObserver(this);
    //registerObserver(Context->ObjManager->getObjectFromName("ObjectGrid"));

    scene::ISceneManager* smgr = Context->Device->getSceneManager();
    Node = smgr->addMeshSceneNode(getPyramidMesh());
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->ItemCube);

    Node->setPosition(pos);
}

ObjectItemPyramid::~ObjectItemPyramid()
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

void ObjectItemPyramid::onMessage(SMessage msg)
{
    if (msg.Type == EMT_UPDATE)
    {
        if (State == EIS_ITEM)
        {
            Node->setRotation(Node->getRotation() + core::vector3df(0, RotSpeed, 0) * msg.Update.Delta);
        }
        else if (State == EIS_EFFECT_FADEIN)
        {
            Context->Mtls->GridCB->setTransform(Context->Mtls->GridCB->getTransform() + msg.Update.fDelta);
            Context->Mtls->GridBackCB->setTransform(Context->Mtls->GridBackCB->getTransform() + msg.Update.fDelta);
            Context->Mtls->CubeCB->setTransform(Context->Mtls->CubeCB->getTransform() + msg.Update.fDelta);

            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= TimeActive - 1)
                State = EIS_EFFECT;
        }
        else if (State == EIS_EFFECT)
        {
            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= 1)
                State = EIS_EFFECT_FADEOUT;
        }
        else if (State == EIS_EFFECT_FADEOUT)
        {
            Context->Mtls->GridCB->setTransform(Context->Mtls->GridCB->getTransform() - msg.Update.fDelta);
            Context->Mtls->GridBackCB->setTransform(Context->Mtls->GridBackCB->getTransform() - msg.Update.fDelta);
            Context->Mtls->CubeCB->setTransform(Context->Mtls->CubeCB->getTransform() - msg.Update.fDelta);

            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= 0)
                delete this;
        }

    }
    else if (msg.Type == EMT_OBJ_POS)
    {
        if (Node->getPosition().Z + DestroyDist <= msg.Position.Z)
        {
            delete this;
            return;
        }
        if ((Node->getPosition() - core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z)).getLength() <= Size*1.5)
        {
            broadcastMessage(SMessage(this, EMT_PLAYER_CUBED));
            EffectCounter = TimeActive;
            State = EIS_EFFECT_FADEIN;

            Node->remove();
            Context->ObjManager->getObjectFromName("ObjectPlayer")->unregisterObserver(this);
        }
    }
}

scene::IMesh* ObjectItemPyramid::getPyramidMesh()
{
    scene::IAnimatedMesh* mesh = Context->Device->getSceneManager()->getMesh("pyramid-mesh");
    if (!mesh)
    {
        GeometryGenerator geomGen;
        mesh = static_cast<scene::IAnimatedMesh*>(geomGen.createPyramidMesh(Size, true));
        Context->Device->getSceneManager()->getMeshCache()->addMesh("pyramid-mesh", mesh);
        mesh->drop();
    }

    return mesh;
}
