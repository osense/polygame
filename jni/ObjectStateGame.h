#ifndef OBJECTSTATEGAME_H
#define OBJECTSTATEGAME_H

#include "Object.h"
#include "ObjectManager.h"

#include "ObjectGrid.h"
#include "ObjectSky.h"
#include "ObjectPlayer.h"


class ObjectStateGame : public Object
{
    public:
        ObjectStateGame(SContext* cont);
        virtual ~ObjectStateGame();

        virtual void onMessage(SMessage msg);

    private:
};

#endif // OBJECTSTATEGAME_H
