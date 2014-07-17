#ifndef OBJECTSTATEGAME_H
#define OBJECTSTATEGAME_H

#include <irrlicht.h>
#include "Object.h"
#include "ObjectManager.h"

#include "functions.h"
#include "ObjectStateInit.h"
#include "ObjectGrid.h"
#include "ObjectItemSpawner.h"
#include "ObjectSky.h"
#include "ObjectPlayer.h"

enum E_GAME_GUI_ID
{
    EGGI_OK = 0
};

using namespace irr;

class ObjectStateGame : public Object
{
    public:
        ObjectStateGame(SContext* cont);
        virtual ~ObjectStateGame();

        virtual void onMessage(SMessage msg);

    private:

        gui::IGUIStaticText* DistTravelledText;
        f32 DT;

        gui::IGUIWindow* GameoverWnd;
        void createGameoverWindow();
};

#endif // OBJECTSTATEGAME_H
