#ifndef OBJECTVISUAL_H
#define OBJECTVISUAL_H

#include "Object.h"
#include <irrlicht.h>

struct SContext;

using namespace irr;

class ObjectVisual : public Object
{
    public:
        ObjectVisual(SContext* cont);
        virtual ~ObjectVisual();

        scene::ISceneNode* getNode();
        virtual void render();

    protected:
        scene::ISceneNode* Node;
};

#endif // OBJECTVISUAL_H
