#include "ObjectVisual.h"

ObjectVisual::ObjectVisual(SContext* cont) : Object(cont)
{
    //ctor
}

ObjectVisual::~ObjectVisual()
{
    //dtor
}

scene::ISceneNode* ObjectVisual::getNode()
{
    return Node;
}

void ObjectVisual::render()
{
    Node->render();
}
