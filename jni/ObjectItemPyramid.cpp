#include "ObjectItemPyramid.h"

ObjectItemPyramid::ObjectItemPyramid(SContext* cont, core::vector3df pos, bool negative) : ObjectItem(cont)
{
    Name = "ObjectItemPyramid";
    Context->ObjManager->broadcastMessage(SMessage(this, EMT_OBJ_SPAWNED));

    Context->ObjManager->getObjectFromName("ObjectUpdater")->registerObserver(this);
    Context->ObjManager->getObjectFromName("ObjectPlayer")->registerObserver(this);

    scene::ISceneManager* smgr = Context->Device->getSceneManager();
    Node = smgr->addMeshSceneNode(smgr->getMesh("pyramid-mesh"));
    Node->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
    Node->setMaterialType(Context->Mtls->Solid);
    Node->getMaterial(0).Thickness = 1.05;

    if (negative)
        Negativity = -1;
    else
        Negativity = 1;

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
            Node->setRotation(Node->getRotation() + core::vector3df(0, RotSpeed, 0) * msg.Update.fDelta);
        }
        else if (State == EIS_EFFECT_FADEIN)
        {
            f32 newT = Context->Mtls->GridCB->getTransform() + msg.Update.fDelta * (1/TimeFadein) * Negativity;

            Context->Mtls->GridCB->setTransform(newT);
            Context->Mtls->GridBackCB->setTransform(newT);
            Context->Mtls->SolidCB->setTransform(newT);

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
            f32 newT = Context->Mtls->GridCB->getTransform() - msg.Update.fDelta * (1/TimeFadein) * Negativity;

            Context->Mtls->GridCB->setTransform(newT);
            Context->Mtls->GridBackCB->setTransform(newT);
            Context->Mtls->SolidCB->setTransform(newT);

            EffectCounter -= msg.Update.fDelta;

            if (EffectCounter <= 0)
            {
                Context->Mtls->GridCB->setTransform(0);
                Context->Mtls->GridBackCB->setTransform(0);
                Context->Mtls->SolidCB->setTransform(0);

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
        if ((Node->getPosition() - core::vector3df(msg.Position.X, msg.Position.Y, msg.Position.Z)).getLength() <= Size*1.5)
        {
            broadcastMessage(SMessage(this, EMT_PLAYER_CUBED));
            if (Context->Mtls->GridCB->getTransform() != 0)
            {
                delete this;
                return;
            }

            EffectCounter = TimeActive;
            State = EIS_EFFECT_FADEIN;

            Node->remove();
            Context->ObjManager->getObjectFromName("ObjectPlayer")->unregisterObserver(this);
        }
    }
}
